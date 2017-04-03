#include "get_capabilities.hpp"
#include "../core/data_types.hpp"
#include "../manager.hpp"

// RapidXML
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>

eows::ogc::wcs::operations::get_capabilities::get_capabilities()
  : eows::ogc::wcs::core::operation()
{

}

eows::ogc::wcs::operations::get_capabilities::~get_capabilities()
{

}

const char*eows::ogc::wcs::operations::get_capabilities::content_type() const
{
  return "application/gml+xml";
}

const std::string eows::ogc::wcs::operations::get_capabilities::to_string() const
{
  const eows::ogc::wcs::core::capabilities_t capabilities = manager::instance().capabilities();

  rapidxml::xml_document<> xml_doc;

  // Preparing Meta document
  rapidxml::xml_node<>* decl = xml_doc.allocate_node(rapidxml::node_declaration);
  decl->append_attribute(xml_doc.allocate_attribute("version", "1.0"));
  decl->append_attribute(xml_doc.allocate_attribute("encoding", "UTF-8"));
  xml_doc.append_node(decl);
  // Preparing WCS Root Element
  rapidxml::xml_node<>* wcs_document = xml_doc.allocate_node(rapidxml::node_element, "wcs:WCSCapabilities");
  wcs_document->append_attribute(xml_doc.allocate_attribute("version", capabilities.service.service_type_version.c_str()));
  wcs_document->append_attribute(xml_doc.allocate_attribute("xmlns","http://www.opengis.net/ows/2.0"));
  wcs_document->append_attribute(xml_doc.allocate_attribute("xmlns:ows","http://www.opengis.net/ows/2.0"));
  wcs_document->append_attribute(xml_doc.allocate_attribute("xmlns:wcs","http://www.opengis.net/wcs/2.0"));
  wcs_document->append_attribute(xml_doc.allocate_attribute("xmlns:gml","http://www.opengis.net/gml/3.2"));
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

  for(auto profile: capabilities.service.profiles)
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
  // Attribute Iterator
  rapidxml::xml_attribute<>* attr_it = xml_doc.allocate_attribute("name", "GetCapabilities");
  child = xml_doc.allocate_node(rapidxml::node_element, "ows:OperationsMetadata");
  // GetCapabilities Operation
  sub_child = xml_doc.allocate_node(rapidxml::node_element, "ows:Operation");
  sub_child->append_attribute(attr_it);
  rapidxml::xml_node<>* dcp_node = xml_doc.allocate_node(rapidxml::node_element, "ows:DCP");
  rapidxml::xml_node<>* http_node = xml_doc.allocate_node(rapidxml::node_element, "ows:HTTP");
  attr_it = xml_doc.allocate_attribute("xlink:href", "http://localhost:7654/wcs");// TODO: apply it dynammically
  http_node->append_attribute(attr_it);
  dcp_node->append_node(http_node);
  sub_child->append_node(dcp_node);

  // ================
  // Service Metadata
  // ================


  std::string buff;
  rapidxml::print(std::back_inserter(buff), xml_doc, 0);

  return buff;
}
