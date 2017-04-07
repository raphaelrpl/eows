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
  \file eows/ogc/wcs/operations/get_capabilities.cpp

  \brief Defines implementation of WCS GetCapabilities operation

  \author Raphael Willian da Costa
 */

// EOWS
#include "describe_coverage.hpp"
#include "../core/data_types.hpp"
#include "data_types.hpp"
#include "../manager.hpp"
#include "../core/utils.hpp"
#include "../core/logger.hpp"

// EOWS GeoArray
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

  /*
   * TODO: Review it
   * String manager. The XML nodes requires a pointer to strings in order to fill values, but with variable scopes,
   * its delete automatically nonpointers. In this way, XML node retrieves inconsistent values. As we only generate
   * XML output after array iteration, we must keep a array of pointers and delete everyone after XML print process
   */
  std::vector<std::string*> string_allocator;

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
      {
        rapidxml::xml_node<>* bound = xml_doc.allocate_node(rapidxml::node_element, "gml:boundedBy");
        coverage->append_node(bound);
        {
          rapidxml::xml_node<>* envelope = xml_doc.allocate_node(rapidxml::node_element, "gml:Envelope");
          // Appending Envelope into bound
          bound->append_node(envelope);

          envelope->append_attribute(xml_doc.allocate_attribute("srsName", "http://www.opengis.net/def/crs/EPSG/0/4326"));
          envelope->append_attribute(xml_doc.allocate_attribute("axisLabels", "Lat Long"));
          envelope->append_attribute(xml_doc.allocate_attribute("srsDimension", "3"));


        }
      }
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
          rapidxml::xml_node<>* grid = xml_doc.allocate_node(rapidxml::node_element, "gml:RectifiedGrid");
          grid->append_attribute(xml_doc.allocate_attribute("gml:id", array.name.c_str()));
          grid->append_attribute(xml_doc.allocate_attribute("dimension", "3"));
          domain_set->append_node(grid);
          {
            rapidxml::xml_node<>* limits = xml_doc.allocate_node(rapidxml::node_element, "gml:limits");
            grid->append_node(limits);

            {
              rapidxml::xml_node<>* grid_envelope = xml_doc.allocate_node(rapidxml::node_element, "gml:GridEnvelope");
              limits->append_node(grid_envelope);

              rapidxml::xml_node<>* elm = xml_doc.allocate_node(rapidxml::node_element, "gml:low");

              // allocating tmp string and storing into string array to delete after XML print
              std::string* low = new std::string(((std::to_string(array.dimensions.x.min_idx) + " ") +
                                                  (std::to_string(array.dimensions.y.min_idx) + " ") +
                                                  std::to_string(array.dimensions.t.min_idx)).c_str());
              elm->value(low->c_str());


              std::string* high = new std::string(std::to_string(array.dimensions.x.max_idx) + " " +
                                                  std::to_string(array.dimensions.y.max_idx) + " " +
                                                  std::to_string(array.dimensions.t.max_idx));

              string_allocator.push_back(low);
              string_allocator.push_back(high);

              grid_envelope->append_node(elm);
              elm = xml_doc.allocate_node(rapidxml::node_element, "gml:high", high->c_str());
              grid_envelope->append_node(elm);
            }

            limits->append_node(xml_doc.allocate_node(rapidxml::node_element, "gml:axisLabels", "x y t"));
          }
        }
      }
      // Preparing RangeSet
      {
        rapidxml::xml_node<>* range_type = xml_doc.allocate_node(rapidxml::node_element, "gmlcov:rangeType");
        coverage->append_node(range_type);
        rapidxml::xml_node<>* data_record = xml_doc.allocate_node(rapidxml::node_element, "swe:DataRecord");
        range_type->append_node(data_record);
        for(const geoarray::attribute_t& attribute: array.attributes)
        {
          rapidxml::xml_node<>* field = xml_doc.allocate_node(rapidxml::node_element, "swe:field");
          data_record->append_node(field);
          field->append_attribute(xml_doc.allocate_attribute("name", attribute.name.c_str()));
          rapidxml::xml_node<>* quantity = xml_doc.allocate_node(rapidxml::node_element, "swe:Quantity");
          field->append_node(quantity);
          // Alloc swe description
          quantity->append_node(xml_doc.allocate_node(rapidxml::node_element,
                                                      "swe:Description",
                                                      attribute.description.c_str()));
          // Alloc swe uom
          rapidxml::xml_node<>* uom = xml_doc.allocate_node(rapidxml::node_element, "swe:uom");
          quantity->append_node(uom);
          uom->append_attribute(xml_doc.allocate_attribute("code", attribute.name.c_str()));

          rapidxml::xml_node<>* constraint = xml_doc.allocate_node(rapidxml::node_element, "swe:constraint");
          quantity->append_node(constraint);
          rapidxml::xml_node<>* allowed_value = xml_doc.allocate_node(rapidxml::node_element, "swe:AllowedValues");
          constraint->append_node(allowed_value);

          std::string* interval = new std::string(std::to_string(attribute.valid_range.min_val) + " " +
                                                  std::to_string(attribute.valid_range.max_val));
          allowed_value->append_node(xml_doc.allocate_node(rapidxml::node_element,
                                                           "swe:interval",
                                                           interval->c_str()));
          // pushing interval to remove later
          string_allocator.push_back(interval);
        }
      }
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

  // deleting allocated string
  for(const std::string* str: string_allocator)
    delete str;

  EOWS_LOG_DEBUG(pimpl_->xml_representation);
}

const char*eows::ogc::wcs::operations::describe_coverage::content_type() const
{
  return pimpl_->format.c_str();
}

const std::string eows::ogc::wcs::operations::describe_coverage::to_string() const
{
  return pimpl_->xml_representation;
}
