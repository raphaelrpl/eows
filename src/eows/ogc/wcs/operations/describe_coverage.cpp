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
  \file eows/ogc/wcs/operations/describe_coverage.cpp

  \brief Defines implementation of WCS DescribeCoverage operation

  \author Raphael Willian da Costa
 */

// EOWS
#include "describe_coverage.hpp"
#include "../core/data_types.hpp"
#include "data_types.hpp"
#include "../manager.hpp"
#include "../core/utils.hpp"
#include "../../../core/logger.hpp"

// EOWS GeoArray
#include "../../../geoarray/data_types.hpp"
#include "../../../geoarray/geoarray_manager.hpp"

// RapidXML
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>

struct eows::ogc::wcs::operations::describe_coverage::impl
{
  impl(const describe_coverage_request& req)
    : request(req), format("application/xml")
  {

  }

  eows::ogc::wcs::operations::describe_coverage_request request;
  std::string xml_representation;
  std::string format;
};

eows::ogc::wcs::operations::describe_coverage::describe_coverage(const describe_coverage_request& req)
  : eows::ogc::wcs::core::operation(),
    pimpl_(new eows::ogc::wcs::operations::describe_coverage::impl(req))
{

}

eows::ogc::wcs::operations::describe_coverage::~describe_coverage()
{
  delete pimpl_;
}

void eows::ogc::wcs::operations::describe_coverage::execute()
{
  rapidxml::xml_document<> xml_doc;

  // Preparing Meta document
  rapidxml::xml_node<>* decl = xml_doc.allocate_node(rapidxml::node_declaration);
  decl->append_attribute(xml_doc.allocate_attribute("version", "1.0"));
  decl->append_attribute(xml_doc.allocate_attribute("encoding", "UTF-8"));
  xml_doc.append_node(decl);
  // Preparing WCS Root Element
  rapidxml::xml_node<>* wcs_document = xml_doc.allocate_node(rapidxml::node_element, "wcs:CoverageDescriptions");
  wcs_document->append_attribute(xml_doc.allocate_attribute("xmlns:wcs","http://www.opengis.net/wcs/2.0"));
  wcs_document->append_attribute(xml_doc.allocate_attribute("xmlns:ows","http://www.opengis.net/ows/2.0"));
  wcs_document->append_attribute(xml_doc.allocate_attribute("xmlns:gml","http://www.opengis.net/gml/3.2"));
  wcs_document->append_attribute(xml_doc.allocate_attribute("xmlns:gmlcov", "http://www.opengis.net/gmlcov/1.0"));
  wcs_document->append_attribute(xml_doc.allocate_attribute("xmlns:xlink", "http://www.w3.org/1999/xlink"));
  wcs_document->append_attribute(xml_doc.allocate_attribute("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance"));
  wcs_document->append_attribute(xml_doc.allocate_attribute("xmlns:swe", "http://www.opengis.net/swe/2.0"));
  wcs_document->append_attribute(xml_doc.allocate_attribute("xsi:schemaLocation"," http://www.opengis.net/wcs/2.0 http://schemas.opengis.net/wcs/2.0/wcsDescribeCoverage.xsd"));
  xml_doc.append_node(wcs_document);

  for(const std::string& array_name: pimpl_->request.coverages_id)
  {
    try
    {
      // Retrived GeoArray metadata
      const eows::geoarray::geoarray_t& array = geoarray::geoarray_manager::instance().get(array_name);

      const eows::ogc::wcs::core::capabilities_t capabilities = manager::instance().capabilities();

      rapidxml::xml_node<>* coverage = xml_doc.allocate_node(rapidxml::node_element, "wcs:CoverageDescription");
      coverage->append_attribute(xml_doc.allocate_attribute("gml:id", array.name.c_str()));

      // Preparing Bounds
      eows::ogc::wcs::core::make_coverage_bounded_by(&xml_doc, coverage, array);
      // Preparing CoverageID
      {
        rapidxml::xml_node<>* coverage_id = xml_doc.allocate_node(rapidxml::node_element,
                                                                  "wcs:CoverageId",
                                                                  array.name.c_str());
        coverage->append_node(coverage_id);
      }
      // Preparing domainSet
      {
        rapidxml::xml_node<>* domain_set = xml_doc.allocate_node(rapidxml::node_element, "gml:domainSet");
        coverage->append_node(domain_set);
        {
          rapidxml::xml_node<>* grid = xml_doc.allocate_node(rapidxml::node_element, "gml:Grid");
          grid->append_attribute(xml_doc.allocate_attribute("gml:id", array.name.c_str()));
          grid->append_attribute(xml_doc.allocate_attribute("dimension", "3"));
          domain_set->append_node(grid);
          {
            rapidxml::xml_node<>* limits = xml_doc.allocate_node(rapidxml::node_element, "gml:limits");
            grid->append_node(limits);

            {
              rapidxml::xml_node<>* grid_envelope = xml_doc.allocate_node(rapidxml::node_element, "gml:GridEnvelope");
              limits->append_node(grid_envelope);

              std::string low = (std::to_string(array.dimensions.x.min_idx) + " ") +
                                (std::to_string(array.dimensions.y.min_idx) + " ") +
                                 std::to_string(array.dimensions.t.min_idx);
              rapidxml::xml_node<>* elm = xml_doc.allocate_node(rapidxml::node_element,
                                                                "gml:low",
                                                                xml_doc.allocate_string(low.c_str()));

              std::string high = (std::to_string(array.dimensions.x.max_idx) + " " +
                                  std::to_string(array.dimensions.y.max_idx) + " " +
                                  std::to_string(array.dimensions.t.max_idx));

              grid_envelope->append_node(elm);
              elm = xml_doc.allocate_node(rapidxml::node_element,
                                          "gml:high",
                                          xml_doc.allocate_string(high.c_str()));
              grid_envelope->append_node(elm);
            }

            limits->append_node(xml_doc.allocate_node(rapidxml::node_element, "gml:axisLabels", "x y t"));
          }
        }
      }
      // Preparing RangeSet
      eows::ogc::wcs::core::make_coverage_range_type(&xml_doc, coverage, array);

      // Preparing Service Parameters
      {
        rapidxml::xml_node<>* parameters = xml_doc.allocate_node(rapidxml::node_element, "wcs:ServiceParameters");
        coverage->append_node(parameters);
        // TODO: Is it dynamically?
        parameters->append_node(xml_doc.allocate_node(rapidxml::node_element, "wcs:CoverageSubtype", "RectifiedGridCoverage"));
        parameters->append_node(xml_doc.allocate_node(rapidxml::node_element, "wcs:nativeFormat", "image/tiff"));
      }
      // Appending CoverageDescription into WCS document
      wcs_document->append_node(coverage);
    }
    catch(const std::invalid_argument& e)
    {
      EOWS_LOG_DEBUG(e.what());
      throw invalid_parameter_error(e.what(), "CoverageNotFound");
    }
  }

  rapidxml::print(std::back_inserter(pimpl_->xml_representation), xml_doc, 0);

  EOWS_LOG_DEBUG(pimpl_->xml_representation);
}

const char*eows::ogc::wcs::operations::describe_coverage::content_type() const
{
  return pimpl_->format.c_str();
}

const std::string& eows::ogc::wcs::operations::describe_coverage::to_string() const
{
  return pimpl_->xml_representation;
}
