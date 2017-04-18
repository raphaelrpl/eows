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

// EOWS Logger
#include "../../../core/logger.hpp"

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
#include "../../../proj4/srs.hpp"

// RapidXML
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>

// SciDB
#include <SciDBAPI.h>

// Boost
#include <boost/shared_ptr.hpp>


thread_local eows::proj4::spatial_ref_map t_srs_idx;

struct scoped_query
{
  boost::shared_ptr< ::scidb::QueryResult > qresult;
  eows::scidb::connection* conn;

  scoped_query(boost::shared_ptr< ::scidb::QueryResult > qr, eows::scidb::connection* c)
    : qresult(std::move(qr)), conn(c)
  {
  }

  ~scoped_query()
  {
    try
    {
      if(qresult != nullptr)
        conn->completed(qresult->queryID);
    }
    catch(...)
    {
      EOWS_LOG_ERROR("scoped_query destructor is throwing exception!");
    }
  }
};


// struct Impl implementation
struct eows::ogc::wcs::operations::get_coverage::impl
{
  impl(const eows::ogc::wcs::operations::get_coverage_request& req)
    : request(req), output()
  {

  }

  /*!
   * \brief It tries to find a geoarray dimension with client subsets. If found, use client subset limits. Otherwise, set default
   * to Geo array
   *
   * \param dimensions - Dimensions Array to search. It must be client limits in WCS request or array default
   * \param target - Target dimension to find
   * \param min_value - Min value to append
   * \param max_value - Max value to append
   */
  void format_dimension_limits(const std::vector<eows::geoarray::dimension_t> dimensions,
                               const eows::geoarray::dimension_t& target,
                               std::string& min_value,
                               std::string& max_value);

  /*!
   * \brief It reprojects a dimension in lat long mode to Grid scale mode in order to retrieve correct array values
   * \todo Implement it
   * \param array - Geo array with dimensions
   * \param srid - SRID to convert
   * \param x - Value of X to reproject
   * \param y - Value of Y to reproject
   */
  void reproject(eows::geoarray::geoarray_t& array, const std::size_t& srid, double& x, double& y);

  //!< Represents WCS client arguments given. TODO: Use it as smart-pointer instead a const value
  const eows::ogc::wcs::operations::get_coverage_request request;
  //!< Represents a cast of array/client subset in lat/long mode to Grid scale mode based in SRID
  std::vector<eows::geoarray::dimension_t> grid_subset;
  //!< Represents WCS GetCoverage output in GML format. TODO: Handle it as binary stream in order to enable image formats
  std::string output;
};

void eows::ogc::wcs::operations::get_coverage::impl::format_dimension_limits(const std::vector<eows::geoarray::dimension_t> dimensions,
                                                                             const eows::geoarray::dimension_t& target,
                                                                             std::string& min_value,
                                                                             std::string& max_value)
{
  // Finding respective array dimension from client limits based in target (geoarray) dimension
  auto dit = geoarray::find_by_name(dimensions, target.name);
  if (dit != dimensions.end())
  {
    min_value += std::to_string(dit->min_idx);
    max_value += std::to_string(dit->max_idx);
  }
  else
  {
    // Setting defaults
    min_value += std::to_string(target.min_idx);
    max_value += std::to_string(target.max_idx);
  }
}

void eows::ogc::wcs::operations::get_coverage::impl::reproject(eows::geoarray::geoarray_t& array,
                                                               const std::size_t& srid,
                                                               double& x,
                                                               double& y)
{
  eows::proj4::spatial_reference* src_srs = nullptr;
  eows::proj4::spatial_reference* dst_srs = nullptr;

  // Performs re-projection of values if the SRID are different
  if (request.input_crs != array.srid)
  {
    // First of all, we must reproject client subsets to an compatible array
    eows::proj4::spatial_ref_map::const_iterator it = t_srs_idx.find(request.input_crs);

    // If it already indexed
    if(it != t_srs_idx.end())
    {
      src_srs = it->second.get();
    }
    else
    {
      // Retrieving GeoArray SRS (dst)
      const eows::proj4::srs_description_t& srs_desc = eows::proj4::srs_manager::instance().get(request.input_crs);
      std::unique_ptr<eows::proj4::spatial_reference> srs(new eows::proj4::spatial_reference(srs_desc.proj4_txt));

      src_srs = srs.get();
      t_srs_idx.insert(std::make_pair(request.input_crs, std::move(srs)));
    }

    eows::proj4::transform(*src_srs, *dst_srs, x, y);
  }
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

std::vector<eows::ogc::wcs::core::subset_t>::const_iterator get(const std::vector<eows::ogc::wcs::core::subset_t>& arr,
                                                                const std::string& name)
{
  for(auto it = arr.begin(); it != arr.end(); ++it)
    if (it->name == name)
      return it;
  return arr.end();
}

void eows::ogc::wcs::operations::get_coverage::execute()
{
  try
  {
    // Retrieve GeoArray information
    const geoarray::geoarray_t& array = geoarray::geoarray_manager::instance().get(pimpl_->request.coverage_id);

    // Array containing client limits sent in WCS request that will be used to build SciDB AFL query
    std::vector<eows::geoarray::dimension_t> dimensions_to_query;

    eows::geoarray::grid grid_array(&array);

    if (pimpl_->request.subsets.size() > 0)
    {
      // Now, we offer coverage 3 dimensions. In this case, we former harded code with this value to process GetCoverage. TODO: change it
      for(const eows::ogc::wcs::core::subset_t& client_subset: pimpl_->request.subsets)
      {
        const double latitude = client_subset.min;
        const double longitude = client_subset.max;
        // Col_id
        if(client_subset.name == array.dimensions.x.name)
        {
          if (!array.spatial_extent.intersects(latitude, longitude))
            throw eows::ogc::wcs::invalid_axis_error("Values does not intersects");

          eows::geoarray::dimension_t d;
          d.name = client_subset.name;
          d.min_idx = grid_array.col(latitude);
          d.max_idx = grid_array.col(longitude);
          dimensions_to_query.push_back(d);
        }
        // Row ID
        else if(client_subset.name == array.dimensions.y.name)
        {
          if (!array.spatial_extent.intersects(latitude, longitude))
            throw eows::ogc::wcs::invalid_axis_error("Values does not intersects");

          eows::geoarray::dimension_t d;
          d.name = client_subset.name;
          d.min_idx = grid_array.row(latitude);
          d.max_idx = grid_array.row(longitude);
          dimensions_to_query.push_back(d);
        }
        // Time ID
        else if(client_subset.name == array.dimensions.t.name)
        {
          if (client_subset.min < array.dimensions.t.min_idx ||
              client_subset.max > array.dimensions.t.max_idx)
            throw eows::ogc::wcs::invalid_axis_error("Time axis does not intersects. " + client_subset.name);

          eows::geoarray::dimension_t time_dimension;
          time_dimension.name = client_subset.name;
          time_dimension.min_idx = client_subset.min;
          time_dimension.max_idx = client_subset.max;
          dimensions_to_query.push_back(time_dimension);
        }
        else
          throw eows::ogc::wcs::invalid_axis_error("No axis found. " + client_subset.name);

        // auto dimension = pimpl_->retrieve_grid_dimension(reprojected_subset);
        // dimensions_to_query.push_back(dimension)
      } // end for
    }

    // Open SciDB connection
    eows::scidb::connection conn = eows::scidb::connection_pool::instance().get(array.cluster_id);

    // Preparing SciDB query string
    std::string query_str = "subarray(" + array.name + ", ";

    // Defining helpers for Query generation
    std::string min_values;
    std::string max_values;

    // Finding X
    pimpl_->format_dimension_limits(dimensions_to_query, array.dimensions.x, min_values, max_values);
    min_values += ", ";
    max_values += ", ";
    // Finding Y
    pimpl_->format_dimension_limits(dimensions_to_query, array.dimensions.y, min_values, max_values);
    min_values += ", ";
    max_values += ", ";
    // Finding T
    pimpl_->format_dimension_limits(dimensions_to_query, array.dimensions.t, min_values, max_values);

    // Generating SciDB AFL statement
    query_str +=  min_values + ", " + max_values + ")";

    boost::shared_ptr<::scidb::QueryResult> query_result = conn.execute(query_str);

    scoped_query sc(query_result, &conn);

    if((query_result == nullptr) || (query_result->array == nullptr))
      throw eows::scidb::query_execution_error("Error in SciDB query result");

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
