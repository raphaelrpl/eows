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
  \file eows/ogc/wms/utils.cpp

  \brief Utility function for the WMS module.

  \author Gilberto Ribeiro de Queiroz
 */

// EOWS
#include "utils.hpp"
#include "../../exception.hpp"
#include "../../core/utils.hpp"

// EOWS OWS
#include "../ows/manager.hpp"

// STL
#include <iterator>

void
eows::ogc::wms::read(const rapidjson::Value& jcapabilities,
                     capabilities_t& capabilities)
{
  if(!jcapabilities.IsObject())
    throw eows::parse_error("Key 'WMS_Capabilities' must be a valid JSON object.");

  rapidjson::Value::ConstMemberIterator jit = jcapabilities.FindMember("Service");

  if(jit == jcapabilities.MemberEnd())
    throw eows::parse_error("Key 'Service' was not found in JSON document.");

  read(jit->value, capabilities.service);

  jit = jcapabilities.FindMember("Capability");

  if(jit == jcapabilities.MemberEnd())
    throw eows::parse_error("Key 'Capability' was not found in JSON document.");

  read(jit->value, capabilities.capability);
}

void
eows::ogc::wms::read(const rapidjson::Value& jservice, service_t& service)
{
  if(!jservice.IsObject())
    throw eows::parse_error("Key 'Service' must be a valid JSON object.");

  service.title = eows::core::read_node_as_string(jservice, "Title");
  service.abstract = eows::core::read_node_as_string(jservice, "Abstract");

  rapidjson::Value::ConstMemberIterator jit = jservice.FindMember("KeywordList");

  if((jit == jservice.MemberEnd()) || (!jit->value.IsArray()))
    throw eows::parse_error("Please, check the key 'Service/KeywordList' in JSON document.");

  eows::core::copy_string_array(jit->value, std::back_inserter(service.keyword_list));

  service.online_resource.href = eows::core::read_node_as_string(jservice, "OnlineResource");

  service.provider = eows::ogc::ows::manager::instance().provider();

  jit = jservice.FindMember("MaxWidth");

  if((jit == jservice.MemberEnd()) || (!jit->value.IsUint()))
    throw eows::parse_error("Please, check the key 'Service/MaxWidth' in JSON document.");

  service.max_width = jit->value.GetUint();

  jit = jservice.FindMember("MaxHeight");

  if((jit == jservice.MemberEnd()) || (!jit->value.IsUint()))
    throw eows::parse_error("Please, check the key 'Service/MaxHeight' in JSON document.");

  service.max_height = jit->value.GetUint();
}

void
eows::ogc::wms::read(const rapidjson::Value& jcapability, capability_t& capability)
{
  if(!jcapability.IsObject())
    throw eows::parse_error("Key 'Capability' must be a valid JSON object.");

  rapidjson::Value::ConstMemberIterator jit = jcapability.FindMember("Request");

  if(jit == jcapability.MemberEnd())
    throw eows::parse_error("Please, check the key 'Capability/Request' in JSON document.");

  read(jit->value, capability.request);

  jit = jcapability.FindMember("Exception");

  if(jit == jcapability.MemberEnd())
    throw eows::parse_error("Please, check the key 'Capability/Exception' in JSON document.");

  read(jit->value, capability.exception);

  jit = jcapability.FindMember("Layer");

  if(jit == jcapability.MemberEnd())
    throw eows::parse_error("Please, check the key 'Capability/Layer' in JSON document.");

  read(jit->value, capability.layer);
}

void
eows::ogc::wms::read(const rapidjson::Value& jrequest, request_t& request)
{

}

void
eows::ogc::wms::read(const rapidjson::Value& jexception, exception_t& exception)
{

}

void
eows::ogc::wms::read(const rapidjson::Value& jlayer, layer_t& layer)
{

}
