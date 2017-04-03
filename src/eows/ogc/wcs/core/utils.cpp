#include "utils.hpp"
#include "data_types.hpp"
#include "../exception.hpp"


void eows::ogc::wcs::core::read(const rapidjson::Value& doc, capabilities_t& capability)
{
  // Reading First Node "WCSCapabilities"
  if(!doc.IsObject())
    throw eows::parse_error("Key 'WCSCapabilities' must be a valid JSON object.");

  // Reading WCS Service Identification
  rapidjson::Value::ConstMemberIterator jit = doc.FindMember("ServiceIdentification");

  if(jit == doc.MemberEnd())
    throw eows::parse_error("Key 'ServiceIdentification' was not found in JSON document.");

  read(jit->value, capability.service);

  // Reading WCS Service Provider
  jit = doc.FindMember("ServiceProvider");

  if(jit == doc.MemberEnd())
    throw eows::parse_error("Key 'ServiceProvider' was not found in JSON document.");

  read(jit->value, capability.service_provider);

  // Reading WCS Service Metadata
  jit = doc.FindMember("ServiceMetadata");

  if(jit == doc.MemberEnd())
    throw eows::parse_error("Key 'ServiceMetadata' was not found in JSON document.");

  read(jit->value, capability.service_metadata);

  // Reading WCS Contents
  jit = doc.FindMember("Contents");

  if(jit == doc.MemberEnd())
    throw eows::parse_error("Key 'Contents' was not found in JSON document.");

  read(jit->value, capability.content);
}

void eows::ogc::wcs::core::read(const rapidjson::Value& jservice, eows::ogc::wcs::core::service_identification_t& service)
{
  // Validating ServiceIdentification structure (must be an object)
  if(!jservice.IsObject())
    throw eows::parse_error("Key 'ServiceIdentification' must be a valid JSON object.");

  // Reading Elements
  service.title = read_node_as_string(jservice, "Title");
  service.abstract = read_node_as_string(jservice, "Abstract");
  service.service_type = read_node_as_string(jservice, "ServiceType");
  service.service_type_version = read_node_as_string(jservice, "ServiceTypeVersion");
  // Reading KeyWords list
  rapidjson::Value::ConstMemberIterator jit = jservice.FindMember("Profiles");
  // TODO: auto format function in common
  if((jit == jservice.MemberEnd()) || (!jit->value.IsArray()))
    throw eows::parse_error("Please, check the key 'Profiles' in JSON document.");

  for(auto& v: jit->value.GetArray())
    service.profiles.push_back(v.GetString());
}

void eows::ogc::wcs::core::read(const rapidjson::Value& jservice, eows::ogc::wcs::core::service_provider_t& provider)
{
  // Validating ServiceProvider structure (must be an object)
  if(!jservice.IsObject())
    throw eows::parse_error("Key 'ServiceProvider' must be a valid JSON object.");

  // Reading Elements
  provider.provider_name = read_node_as_string(jservice, "ProviderName");
  provider.provider_site = read_node_as_string(jservice, "ProviderSite");

  rapidjson::Value::ConstMemberIterator contact_it = jservice.FindMember("ServiceContact");
  if (contact_it == jservice.MemberEnd())
    throw eows::parse_error("Key 'ServiceContact' were not found in JSON document");

  if (!contact_it->value.IsObject())
    throw eows::parse_error("Key 'ServiceContact' is not a JSON object");

//  rapidjson::Value::ConstMemberIterator it = jservice.FindMember("ServiceContact");

  provider.service_contact.individual_name = read_node_as_string(contact_it->value, "IndividualName");
  provider.service_contact.position_name = read_node_as_string(contact_it->value, "PositionName");
}

const std::string eows::ogc::wcs::core::read_node_as_string(const rapidjson::Value& node, const std::string& memberName)
{
  rapidjson::Value::ConstMemberIterator jit = node.FindMember(memberName.c_str());
  // TODO: auto format function in common
  if((jit == node.MemberEnd()) || (!jit->value.IsString()))
    throw eows::parse_error("Please, check the key " + memberName + " in JSON document.");
  return jit->value.GetString();
}

void eows::ogc::wcs::core::read(const rapidjson::Value& jservice, eows::ogc::wcs::core::service_metadata_t& metadata)
{
  // Validating ServiceProvider structure (must be an object)
  if(!jservice.IsObject())
    throw eows::parse_error("Key 'ServiceMetadata' must be a valid JSON object.");

  rapidjson::Value::ConstMemberIterator jit = jservice.FindMember("formatSupported");
  if((jit == jservice.MemberEnd() || !jit->value.IsArray()))
    throw eows::parse_error("Key 'ServiceMetadata' must be a array.");

  for(auto& format: jit->value.GetArray())
    metadata.formats_supported.push_back(format.GetString());
}

void eows::ogc::wcs::core::read(const rapidjson::Value& jservice, eows::ogc::wcs::core::content_t& content)
{
  // Validating ServiceProvider structure (must be an object)
  if(!jservice.IsArray())
    throw eows::parse_error("Key 'Contents' must be a valid JSON Array.");

  for(auto& format: jservice.GetArray())
  {
    rapidjson::Value::ConstMemberIterator jit = format.FindMember("CoverageSummary");

    eows::ogc::wcs::core::coverage_summary_t coverage_summary;
    coverage_summary.coverage_id = read_node_as_string(jit->value, "id");
    coverage_summary.coverage_subtype = read_node_as_string(jit->value, "subtype");
    content.summaries.push_back(coverage_summary);
  }
}
