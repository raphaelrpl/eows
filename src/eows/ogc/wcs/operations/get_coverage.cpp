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

// EOWS SciDB
#include "../../../scidb/exception.hpp"
#include "../../../scidb/connection.hpp"
#include "../../../scidb/connection_pool.hpp"
#include "../../../scidb/cell_iterator.hpp"

// RapidXML
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>

// SciDB
#include <SciDBAPI.h>

// Boost
#include <boost/shared_ptr.hpp>


struct eows::ogc::wcs::operations::get_coverage::impl
{
  impl(const eows::ogc::wcs::operations::get_coverage_request& req)
    : request(req), output()
  {

  }

  const eows::ogc::wcs::operations::get_coverage_request request;
  std::string output;
};

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

    // Retrieve from SciDB
    eows::scidb::connection conn = eows::scidb::connection_pool::instance().get(array.cluster_id);

    const geoarray::dimensions_t& dimensions = array.dimensions;

    // Preparing SciDB query string
    std::string query_str = "subarray(" + array.name + ", ";
    // Appending Dimension Min
    query_str += std::to_string(dimensions.x.min_idx) + ", " +
                 std::to_string(dimensions.y.min_idx) + ", " +
                 std::to_string(dimensions.t.min_idx) + ",";
    // Appending Dimension Max
    query_str += std::to_string(dimensions.x.max_idx) + ", " +
                 std::to_string(dimensions.y.max_idx) + ", " +
                 std::to_string(dimensions.t.max_idx) + ")";

    boost::shared_ptr<::scidb::QueryResult> query_result = conn.execute(query_str);
    boost::shared_ptr<eows::scidb::cell_iterator> cell_it(new eows::scidb::cell_iterator(query_result->array));

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
