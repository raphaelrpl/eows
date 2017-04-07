#include "utils.hpp"
#include "data_types.hpp"
#include "../exception.hpp"
#include "../../../core/utils.hpp"

#include<algorithm>
#include<sstream>

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

  // Reading WCS Operations Metadata
  jit = doc.FindMember("OperationsMetadata");
  if (jit == doc.MemberEnd())
    throw eows::parse_error("Key 'OperationsMetadata' were not found in JSON document");
  read(jit->value, capability.operation_metadata);
}

void eows::ogc::wcs::core::read(const rapidjson::Value& jservice, eows::ogc::wcs::core::service_identification_t& service)
{
  // Validating ServiceIdentification structure (must be an object)
  if(!jservice.IsObject())
    throw eows::parse_error("Key 'ServiceIdentification' must be a valid JSON object.");

  // Reading Elements
  service.title = eows::core::read_node_as_string(jservice, "Title");
  service.abstract = eows::core::read_node_as_string(jservice, "Abstract");
  service.service_type = eows::core::read_node_as_string(jservice, "ServiceType");
  service.service_type_version = eows::core::read_node_as_string(jservice, "ServiceTypeVersion");
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
  provider.provider_name = eows::core::read_node_as_string(jservice, "ProviderName");
  provider.provider_site = eows::core::read_node_as_string(jservice, "ProviderSite");

  rapidjson::Value::ConstMemberIterator contact_it = jservice.FindMember("ServiceContact");
  if (contact_it == jservice.MemberEnd())
    throw eows::parse_error("Key 'ServiceContact' were not found in JSON document");

  if (!contact_it->value.IsObject())
    throw eows::parse_error("Key 'ServiceContact' is not a JSON object");

//  rapidjson::Value::ConstMemberIterator it = jservice.FindMember("ServiceContact");

  provider.service_contact.individual_name = eows::core::read_node_as_string(contact_it->value, "IndividualName");
  provider.service_contact.position_name = eows::core::read_node_as_string(contact_it->value, "PositionName");
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

void eows::ogc::wcs::core::read(const rapidjson::Value& jservice, eows::ogc::wcs::core::operation_metadata_t& operation_meta)
{
  // Validating OperationsMetadata structure (must be an array)
  if(!jservice.IsArray())
    throw eows::parse_error("Key 'OperationsMetadata' must be a valid JSON Array.");

  for(const rapidjson::Value& json_operation: jservice.GetArray())
  {
    eows::ogc::wcs::core::operation_t op;
    op.name = eows::core::read_node_as_string(json_operation, "name");
    /*
      TODO: Is there other type like Distributed Computing Platforms(DCPs)?
            Support another Operations that just HTTP.. i.e SOAP?
    */
    rapidjson::Value::ConstMemberIterator jit = json_operation.FindMember("DCP");
    if (jit == json_operation.MemberEnd() || !jit->value.IsObject())
      throw eows::parse_error("Key 'DCP' must be a valid JSON object.");

    rapidjson::Value::ConstMemberIterator sub_it = jit->value.FindMember("HTTP");
    if (sub_it == jit->value.MemberEnd() || !sub_it->value.IsObject())
      throw eows::parse_error("Key 'HTTP' must be a valid JSON object.");

    // TODO: Add support for POST
    op.dcp.get = eows::core::read_node_as_string(sub_it->value, "GET");

    // Appending into loaded operations
    operation_meta.operations.push_back(op);
  }
}

std::vector<std::string> eows::ogc::wcs::core::split(const std::string& str, const char& delimiter)
{
  std::vector<std::string> output;
  std::istringstream stream(str);
  std::string tmp;
  while(std::getline(stream, tmp, delimiter))
    output.push_back(tmp);
  return output;
}
