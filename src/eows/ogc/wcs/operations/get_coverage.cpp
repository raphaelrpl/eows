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
    boost::shared_ptr<::scidb::QueryResult> query_result = conn.execute("scan(\"" + array.name + "\")");
    boost::shared_ptr<eows::scidb::cell_iterator> cell_it(new eows::scidb::cell_iterator(query_result->array));

    std::ostringstream ss;

    while(!cell_it->end())
    {
      auto coordinates = cell_it->get_position();

      for(std::size_t attr_pos = 0; attr_pos < array.attributes.size(); ++attr_pos)
      {
        const geoarray::attribute_t& attr = array.attributes[attr_pos];
        if (attr.datatype == geoarray::datatype_t::int16_dt)
          ss << cell_it->get_int16(attr.name) << " ";
        else
          ss << cell_it->get_int8(attr.name) << " ";
      }
      ss << "\n";
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
    wcs_document->append_attribute(xml_doc.allocate_attribute("xsi:schemaLocation",
                                                              "http://www.opengis.net/swe/2.0 http://schemas.opengis.net/sweCommon/2.0/swe.xsd http://www.opengis.net/gmlcov/1.0 http://schemas.opengis.net/gmlcov/1.0/gmlcovAll.xsd"));
    wcs_document->append_attribute(xml_doc.allocate_attribute("gml:id", array.name.c_str()));
    xml_doc.append_node(wcs_document);

    // Preparing bounded by
    eows::ogc::wcs::core::make_coverage_bounded_by(&xml_doc, wcs_document, array);

    // Preparing domainset

    // Preparing rangeset



    if (ss.good())
      pimpl_->output = ss.str();
    else
      pimpl_->output = "notgood";
  }
  catch(const eows::scidb::connection_open_error& e)
  {
    // Todo: Throw UnappliedError
    throw eows::ogc::ogc_error(e.what(), "");
  }
  catch(const std::exception& e)
  {
    throw ogc::ogc_error("No such coverage '" + pimpl_->request.coverage_id + "'", "coverageID");
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
