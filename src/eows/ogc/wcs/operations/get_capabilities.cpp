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
#include "get_capabilities.hpp"
#include "../core/data_types.hpp"
#include "data_types.hpp"
#include "../manager.hpp"
#include "../core/utils.hpp"
// EOWS GeoArray (coverage summaries)
#include "../../../geoarray/data_types.hpp"
#include "../../../geoarray/geoarray_manager.hpp"

// RapidXML
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>

struct eows::ogc::wcs::operations::get_capabilities::impl
{
  impl(const get_capabilities_request& req)
    : request(req), format("application/xml")
  {

  }

  eows::ogc::wcs::operations::get_capabilities_request request;
  std::string xml_representation;
  std::string format;
};

eows::ogc::wcs::operations::get_capabilities::get_capabilities(const get_capabilities_request& req)
  : eows::ogc::wcs::core::operation(),
    pimpl_(new eows::ogc::wcs::operations::get_capabilities::impl(req))
{

}

eows::ogc::wcs::operations::get_capabilities::~get_capabilities()
{

}

void eows::ogc::wcs::operations::get_capabilities::execute()
{
  const eows::ogc::wcs::core::capabilities_t capabilities = manager::instance().capabilities();

  rapidxml::xml_document<> xml_doc;

  // Preparing Meta document
  rapidxml::xml_node<>* decl = xml_doc.allocate_node(rapidxml::node_declaration);
  decl->append_attribute(xml_doc.allocate_attribute("version", "1.0"));
  decl->append_attribute(xml_doc.allocate_attribute("encoding", "UTF-8"));
  xml_doc.append_node(decl);
  // Preparing WCS Root Element
  rapidxml::xml_node<>* wcs_document = xml_doc.allocate_node(rapidxml::node_element, "wcs:Capabilities");
  wcs_document->append_attribute(xml_doc.allocate_attribute("version", capabilities.service.service_type_version.c_str()));
  wcs_document->append_attribute(xml_doc.allocate_attribute("xmlns","http://www.opengis.net/ows/2.0"));
  wcs_document->append_attribute(xml_doc.allocate_attribute("xmlns:ows","http://www.opengis.net/ows/2.0"));
  wcs_document->append_attribute(xml_doc.allocate_attribute("xmlns:wcs","http://www.opengis.net/wcs/2.0"));
  wcs_document->append_attribute(xml_doc.allocate_attribute("xmlns:gml","http://www.opengis.net/gml/3.2"));
  wcs_document->append_attribute(xml_doc.allocate_attribute("xmlns:xlink", "http://www.w3.org/1999/xlink"));
  wcs_document->append_attribute(xml_doc.allocate_attribute("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance"));
  wcs_document->append_attribute(xml_doc.allocate_attribute("xsi:schemaLocation","http://www.opengis.net/wcs/2.0 http://schemas.opengis.net/wcs/2.0/wcsAll.xsd"));
  xml_doc.append_node(wcs_document);

  // OWS Identification
  rapidxml::xml_node<>* ows_identification = xml_doc.allocate_node(rapidxml::node_element, "ows:ServiceIdentification");
  rapidxml::xml_node<>* child = xml_doc.allocate_node(rapidxml::node_element, "ows:Title");

  child->value(capabilities.service.title.c_str());
  ows_identification->append_node(child);

  child = xml_doc.allocate_node(rapidxml::node_element, "ows:Abstract");
  child->value(capabilities.service.abstract.c_str());
  ows_identification->append_node(child);

  child = xml_doc.allocate_node(rapidxml::node_element, "ows:ServiceType");
  child->value(capabilities.service.service_type.c_str());
  ows_identification->append_node(child);

  child = xml_doc.allocate_node(rapidxml::node_element, "ows:ServiceTypeVersion");
  child->value(capabilities.service.service_type_version.c_str());
  ows_identification->append_node(child);

  for(auto& profile: capabilities.service.profiles)
  {
    child = xml_doc.allocate_node(rapidxml::node_element, "ows:Profile");
    child->value(profile.c_str());
    ows_identification->append_node(child);
  }

  wcs_document->append_node(ows_identification);
  // ==============
  // OWS Provider
  // ==============
  rapidxml::xml_node<>* ows_provider = xml_doc.allocate_node(rapidxml::node_element, "ows:ServiceProvider");
  child = xml_doc.allocate_node(rapidxml::node_element, "ows:ProviderName");
  child->value(capabilities.service_provider.provider_name.c_str());
  ows_provider->append_node(child);

  child = xml_doc.allocate_node(rapidxml::node_element, "ows:ProviderSite");
  child->value(capabilities.service_provider.provider_site.c_str());
  ows_provider->append_node(child);

  child = xml_doc.allocate_node(rapidxml::node_element, "ows:ServiceContact");
  const eows::ogc::wcs::core::service_contact_t& contact = capabilities.service_provider.service_contact;
  // Processing Provider -> Contact -> Nodes[]
  rapidxml::xml_node<>* sub_child = xml_doc.allocate_node(rapidxml::node_element, "ows:IndividualName");
  sub_child->value(contact.individual_name.c_str());
  child->append_node(sub_child);

  sub_child = xml_doc.allocate_node(rapidxml::node_element, "ows:PositionName");
  sub_child->value(contact.position_name.c_str());
  child->append_node(sub_child);
  ows_provider->append_node(child);
  wcs_document->append_node(ows_provider);

  // ===================
  // Operations Metadata
  // ===================

  child = xml_doc.allocate_node(rapidxml::node_element, "ows:OperationsMetadata");
  const eows::ogc::wcs::core::operation_metadata_t& operation_metadata = capabilities.operation_metadata;
  for(auto& operation: operation_metadata.operations)
  {
    sub_child = xml_doc.allocate_node(rapidxml::node_element, "ows:Operation");
    sub_child->append_attribute(xml_doc.allocate_attribute("name", operation.name.c_str()));

    rapidxml::xml_node<>* dcp_node = xml_doc.allocate_node(rapidxml::node_element, "ows:DCP");
    rapidxml::xml_node<>* http_node = xml_doc.allocate_node(rapidxml::node_element, "ows:HTTP");
    rapidxml::xml_node<>* get_node = xml_doc.allocate_node(rapidxml::node_element, "ows:Get");
    get_node->append_attribute(xml_doc.allocate_attribute("xlink:href", operation.dcp.get.c_str()));
    http_node->append_node(get_node);
    dcp_node->append_node(http_node);
    sub_child->append_node(dcp_node);
    child->append_node(sub_child);
  }
  wcs_document->append_node(child);

  // ================
  // Service Metadata
  // ================
  child = xml_doc.allocate_node(rapidxml::node_element, "wcs:ServiceMetadata");
  for(auto& format: capabilities.service_metadata.formats_supported)
  {
    sub_child = xml_doc.allocate_node(rapidxml::node_element, "wcs:formatSupported");
    sub_child->value(format.c_str());
    child->append_node(sub_child);
  }
  wcs_document->append_node(child);

  // ========
  // Contents
  // ========
  child = xml_doc.allocate_node(rapidxml::node_element, "wcs:Contents");
  std::vector<std::string> geoarrays = geoarray::geoarray_manager::instance().list_arrays();

  for(const std::string& array_name: geoarrays)
  {
    const geoarray::geoarray_t& array = geoarray::geoarray_manager::instance().get(array_name);

    sub_child = xml_doc.allocate_node(rapidxml::node_element, "wcs:CoverageSummary");
    {
      rapidxml::xml_node<>* coverage_node = xml_doc.allocate_node(rapidxml::node_element, "wcs:CoverageId");
      coverage_node->value(array.name.c_str());
      sub_child->append_node(coverage_node);

      coverage_node = xml_doc.allocate_node(rapidxml::node_element, "wcs:CoverageSubtype");
      // TODO: add it into geoarray_t
      coverage_node->value("GridCoverage");
      sub_child->append_node(coverage_node);
    }
    child->append_node(sub_child);
  }
  wcs_document->append_node(child);

  rapidxml::print(std::back_inserter(pimpl_->xml_representation), xml_doc, 0);
}

const char*eows::ogc::wcs::operations::get_capabilities::content_type() const
{
  return pimpl_->format.c_str();
}

const std::string eows::ogc::wcs::operations::get_capabilities::to_string() const
{
  return pimpl_->xml_representation;
}
