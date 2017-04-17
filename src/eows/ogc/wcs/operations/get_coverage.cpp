/*
  Copyright (C) 2017 National Institute For Space Research (INPE) - Brazil.

  This file is part of Earth Observation Web Services (EOWS).

  EOWS is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3 as
  published by the Free Software Foundation.

  EOWS is distributed  "AS-IS" in the hope that it will be useful,
  but WITHOUT ANY WARRANTY OF ANY KIND; without even the implied warranty
  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with EOWS. See LICENSE. If not, write to
  e-sensing team at <esensing-team@dpi.inpe.br>.
 */

/*!
  \file eows/ogc/wcs/operations/get_coverage.cpp

  \brief Implementation of WCS GetCoverage operation

  \author Raphael Willian da Costa
 */

// EOWS
#include "get_coverage.hpp"
#include "data_types.hpp"
#include "../core/utils.hpp"

// EOWS GeoArray
#include "../../../geoarray/data_types.hpp"
#include "../../../geoarray/geoarray_manager.hpp"
#include "../../../geoarray/utils.hpp"

// EOWS SciDB
#include "../../../scidb/exception.hpp"
#include "../../../scidb/connection.hpp"
#include "../../../scidb/connection_pool.hpp"
#include "../../../scidb/cell_iterator.hpp"

// EOWS Proj4
//#include "../../../proj4/srs.hpp"

// RapidXML
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>

// SciDB
#include <SciDBAPI.h>

// Boost
#include <boost/shared_ptr.hpp>

// struct Impl implementation
struct eows::ogc::wcs::operations::get_coverage::impl
{
  impl(const eows::ogc::wcs::operations::get_coverage_request& req)
    : request(req), output()
  {

  }

  /*!
   * \brief It performs a Geo Array dimensions validate from each one of subsets given by user
   * \param array - Geoarray with meta axis information
   * \throws eows::ogc::wcs::invalid_axis_error - When axis value is invalid
   */
  void validate_subset(const eows::geoarray::geoarray_t& array);

  /*!
   * \brief It checks given subset limits
   * \param subset Subset sent by client
   * \param array_dimension - Geo array dimension of axis
   * \throws eows::ogc::wcs::invalid_axis_error When client given limits does not match array limits
   */
  bool is_valid_subset(const eows::geoarray::dimension_t& subset, const eows::geoarray::dimension_t& array_dimension);

  /*!
   * \brief It tries to find a geoarray dimension with client subsets. If found, use client subset limits. Otherwise, set default
   * to Geo array
   *
   * \param dimension - Target dimension to find
   * \param min_value - Min value to append
   * \param max_value - Max value to append
   */
  void format_dimension_limits(const eows::geoarray::dimension_t& dimension, std::string& min_value, std::string& max_value);

  /*!
   * \brief It reprojects a dimension in lat long mode to Grid scale mode in order to retrieve correct array values
   * \todo Implement it
   * \param dimension - Target dimension to fill
   * \param minx - Min value
   * \param maxx - Max value
   */
  void reproject(eows::geoarray::dimension_t& dimension, const double& minx, const double& maxx);

  //!< Represents WCS client arguments given. TODO: Use it as smart-pointer instead a const value
  const eows::ogc::wcs::operations::get_coverage_request request;
  //!< Represents a cast of array/client subset in lat/long mode to Grid scale mode based in SRID
  std::vector<eows::geoarray::dimension_t> grid_subset;
  //!< Represents WCS GetCoverage output in GML format. TODO: Handle it as binary stream in order to enable image formats
  std::string output;
};

bool eows::ogc::wcs::operations::get_coverage::impl::is_valid_subset(const eows::geoarray::dimension_t & subset,
                                                                     const eows::geoarray::dimension_t & array_dimension)
{
  // If found, check limits
  if (subset.name == array_dimension.name)
  {
    if (subset.min_idx >= array_dimension.min_idx &&
        subset.max_idx <= array_dimension.max_idx)
      return true;

    throw eows::ogc::wcs::invalid_axis_error("Invalid axis limits \"" + subset.name + "\" \"" +
                                             std::to_string(subset.min_idx) + "\" and \"" +
                                             std::to_string(subset.max_idx) + "\".");
  }

  return false;
}

void eows::ogc::wcs::operations::get_coverage::impl::validate_subset(const eows::geoarray::geoarray_t& array)
{
  if (!request.subsets.empty())
  {
    for(const geoarray::dimension_t& subset: request.subsets)
    {
      if (is_valid_subset(subset, array.dimensions.x) ||
          is_valid_subset(subset, array.dimensions.y) ||
          is_valid_subset(subset, array.dimensions.t))
        continue;

      throw eows::ogc::wcs::invalid_axis_error("No axis '" + subset.name + "' found");
    }
  }
}

void eows::ogc::wcs::operations::get_coverage::impl::format_dimension_limits(const eows::geoarray::dimension_t& dimension,
                                                                             std::string& min_value,
                                                                             std::string& max_value)
{
  auto dit = geoarray::find_by_name(request.subsets, dimension.name);
  if (dit != request.subsets.end())
  {
    min_value += std::to_string(dit->min_idx);
    max_value += std::to_string(dit->max_idx);
  }
  else
  {
    // Setting defaults
    min_value += std::to_string(dimension.min_idx);
    max_value += std::to_string(dimension.max_idx);
  }
}

void eows::ogc::wcs::operations::get_coverage::impl::reproject(eows::geoarray::dimension_t& dimension, const double& minx, const double& maxx)
{
  throw eows::ogc::not_implemented_error("Reproject subset in lat/log to Grid is not implemented yet", "");
}

// GetCoverage Implementations

eows::ogc::wcs::operations::get_coverage::get_coverage(const eows::ogc::wcs::operations::get_coverage_request& req)
  : pimpl_(new impl(req))
{

}

eows::ogc::wcs::operations::get_coverage::~get_coverage()
{
  delete pimpl_;
}

void eows::ogc::wcs::operations::get_coverage::execute()
{
  try
  {
    // Retrieve GeoArray information
    const geoarray::geoarray_t& array = geoarray::geoarray_manager::instance().get(pimpl_->request.coverage_id);

    // validate axis given even if client no sent
    pimpl_->validate_subset(array);

    // Retrieve from SciDB
    eows::scidb::connection conn = eows::scidb::connection_pool::instance().get(array.cluster_id);

    const geoarray::dimensions_t& dimensions = array.dimensions;

    // Preparing SciDB query string
    std::string query_str = "subarray(" + array.name + ", ";

    // Defining helpers for Query generation
    std::string min_values;
    std::string max_values;

    // Finding X
    pimpl_->format_dimension_limits(dimensions.x, min_values, max_values);
    min_values += ", ";
    max_values += ", ";
    // Finding Y
    pimpl_->format_dimension_limits(dimensions.y, min_values, max_values);
    min_values += ", ";
    max_values += ", ";
    // Finding T
    pimpl_->format_dimension_limits(dimensions.t, min_values, max_values);

    // Generating SciDB AFL statement
    query_str +=  min_values + ", " + max_values + ")";

    boost::shared_ptr<::scidb::QueryResult> query_result = conn.execute(query_str);
    boost::shared_ptr<eows::scidb::cell_iterator> cell_it(new eows::scidb::cell_iterator(query_result->array));

    // Defining output stream where SciDB will be stored (GML format)
    std::ostringstream ss;
    auto attributes_size = array.attributes.size();

    while(!cell_it->end())
    {
      auto coordinates = cell_it->get_position();

      for(std::size_t attr_pos = 0; attr_pos < attributes_size; ++attr_pos)
      {
        const geoarray::attribute_t& attr = array.attributes[attr_pos];
        if (attr.datatype == geoarray::datatype_t::int16_dt)
          ss << cell_it->get_int16(attr.name);
        else // It is important cast to string to avoid stream inconsistency
          ss << std::to_string(cell_it->get_int8(attr.name));

        if (attr_pos + 1 < attributes_size)
          ss << " ";
      }
      ss << ",";
      cell_it->next();
    }

    // Defining GetCoverage XML document
    rapidxml::xml_document<> xml_doc;

    // Preparing Meta document
    rapidxml::xml_node<>* decl = xml_doc.allocate_node(rapidxml::node_declaration);
    decl->append_attribute(xml_doc.allocate_attribute("version", "1.0"));
    decl->append_attribute(xml_doc.allocate_attribute("encoding", "UTF-8"));
    xml_doc.append_node(decl);
    // Preparing WCS Root Element
    rapidxml::xml_node<>* wcs_document = xml_doc.allocate_node(rapidxml::node_element, "gmlcov:GridCoverage");
    wcs_document->append_attribute(xml_doc.allocate_attribute("version", "2.0.1"));
    wcs_document->append_attribute(xml_doc.allocate_attribute("xmlns:gml","http://www.opengis.net/gml/3.2"));
    wcs_document->append_attribute(xml_doc.allocate_attribute("xmlns","http://www.opengis.net/gml/3.2"));
    wcs_document->append_attribute(xml_doc.allocate_attribute("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance"));
    wcs_document->append_attribute(xml_doc.allocate_attribute("xmlns:gmlcov", "http://www.opengis.net/gmlcov/1.0"));
    wcs_document->append_attribute(xml_doc.allocate_attribute("xmlns:swe", "http://www.opengis.net/swe/2.0"));
    wcs_document->append_attribute(xml_doc.allocate_attribute("xsi:schemaLocation",
                                                              "http://www.opengis.net/swe/2.0 http://schemas.opengis.net/sweCommon/2.0/swe.xsd http://www.opengis.net/gmlcov/1.0 http://schemas.opengis.net/gmlcov/1.0/gmlcovAll.xsd"));
    wcs_document->append_attribute(xml_doc.allocate_attribute("gml:id", array.name.c_str()));
    xml_doc.append_node(wcs_document);

    // Preparing bounded by
    eows::ogc::wcs::core::make_coverage_bounded_by(&xml_doc, wcs_document, array);
    // Preparing domainset
    eows::ogc::wcs::core::make_coverage_domain_set(&xml_doc, wcs_document, array);

    // Preparing rangeset
    rapidxml::xml_node<>* range_set = xml_doc.allocate_node(rapidxml::node_element, "gml:rangeSet");
    wcs_document->append_node(range_set);
    rapidxml::xml_node<>* data_block = xml_doc.allocate_node(rapidxml::node_element, "gml:DataBlock");

    const std::string scidb_data = ss.str();

    rapidxml::xml_node<>* tuple_list = xml_doc.allocate_node(rapidxml::node_element, "gml:tupleList", scidb_data.c_str(), 0, scidb_data.size());
    // Defining delimiter in order to client use to read properly row
    tuple_list->append_attribute(xml_doc.allocate_attribute("ts", ","));
    // Defining delimiter for coverage attribute
    tuple_list->append_attribute(xml_doc.allocate_attribute("cs", " "));

    data_block->append_node(tuple_list);
    range_set->append_node(data_block);

    // Preparing rangetype
    eows::ogc::wcs::core::make_coverage_range_type(&xml_doc, wcs_document, array);

    rapidxml::print(std::back_inserter(pimpl_->output), xml_doc, 0);
  }
  catch(const eows::ogc::ogc_error&)
  {
    throw;
  }
  catch(const eows::scidb::connection_open_error& e)
  {
    // Todo: Throw UnappliedError
    throw eows::ogc::ogc_error(e.what(), "");
  }
  catch(const std::exception& e)
  {
    throw wcs::no_such_coverage_error("No such coverage '" + pimpl_->request.coverage_id + "'");
  }
}

const char*eows::ogc::wcs::operations::get_coverage::content_type() const
{
  return pimpl_->request.format.c_str();
}

const std::string& eows::ogc::wcs::operations::get_coverage::to_string() const
{
  return pimpl_->output;
}
