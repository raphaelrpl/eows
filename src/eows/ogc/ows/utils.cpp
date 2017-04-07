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
  \file eows/ogc/ows/utils.cpp

  \brief Utility functions for OGC OWS module.

  \author Gilberto Ribeiro de Queiroz
 */

// EOWS
#include "utils.hpp"
#include "exception.hpp"
#include "data_types.hpp"
#include "../../core/defines.hpp"
#include "../../core/logger.hpp"
#include "../../core/utils.hpp"



void eows::ogc::ows::read(const rapidjson::Value& node, eows::ogc::ows::provider_t& provider)
{
  if (!node.IsObject())
    throw eows::parse_error("Key 'provider' must be a valid JSON object");

  provider.name = eows::core::read_node_as_string(node, "name");
  provider.site = eows::core::read_node_as_string(node, "site");

  rapidjson::Value::ConstMemberIterator jit = node.FindMember("contact");
  if (jit == node.MemberEnd())
    throw eows::parse_error("Key 'contact' in 'provider' not found");

  read(jit->value, provider.contact);
}

void eows::ogc::ows::read(const rapidjson::Value& node, eows::ogc::ows::contact_t& contact)
{
  if (!node.IsObject())
    throw eows::parse_error("Key 'contact' must be a valid JSON object");

  contact.name= eows::core::read_node_as_string(node, "name");
  contact.position = eows::core::read_node_as_string(node, "position");

  rapidjson::Value::ConstMemberIterator jit = node.FindMember("info");
  if (jit == node.MemberEnd())
    throw eows::parse_error("Key 'info' in 'contact' not found");

  read(jit->value, contact.info);
}

void eows::ogc::ows::read(const rapidjson::Value& node, eows::ogc::ows::contact_info_t& info)
{
  if (!node.IsObject())
    throw eows::parse_error("Key 'info' must be a valid JSON object");

  info.address= eows::core::read_node_as_string(node, "address");
  info.address_type= eows::core::read_node_as_string(node, "address_type");
  info.city= eows::core::read_node_as_string(node, "city");
  info.code= eows::core::read_node_as_string(node, "code");
  info.country= eows::core::read_node_as_string(node, "country");
  info.state= eows::core::read_node_as_string(node, "state");
}
