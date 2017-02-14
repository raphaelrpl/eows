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
  \file eows/http/cppnetlib/utils.cpp

  \brief Utility functions for the HTTP server based on C++ Network Library.

  \author Gilberto Ribeiro de Queiroz
 */

// EOWS
#include "utils.hpp"
#include "../../core/http_server_builder.hpp"
#include "../../core/logger.hpp"
#include "http_server.hpp"

// Boost
#include <boost/tokenizer.hpp>

// C++ Network Library
#include <boost/network/uri.hpp>

eows::core::query_string_t
eows::http::cppnetlib::expand(const std::string& query_str)
{
  eows::core::query_string_t result;

  boost::char_separator<char> kvp_sep("&");

  boost::tokenizer<boost::char_separator<char> > key_value_pairs(query_str, kvp_sep);

  boost::tokenizer<boost::char_separator<char> >::iterator ikv_pair = key_value_pairs.begin();

  while(ikv_pair != key_value_pairs.end())
  {
    std::string kv = *ikv_pair;

    boost::char_separator<char> kv_sep("=");

    boost::tokenizer<boost::char_separator<char> > key_value_pair(kv, kv_sep);

    boost::tokenizer<boost::char_separator<char> >::iterator ikv = key_value_pair.begin();

    if(ikv == key_value_pair.end())
      throw eows::parse_error("Invalid key-value pair format in query string!");

    std::string k = *ikv;

    ++ikv;

    if(ikv == key_value_pair.end())
      throw eows::parse_error("Invalid key-value pair format in query string!");

    std::string v = *ikv;

    std::string decoded_k;
    std::string decoded_v;

    boost::network::uri::decode(k.begin(), k.end(), std::back_inserter(decoded_k));
    boost::network::uri::decode(v.begin(), v.end(), std::back_inserter(decoded_v));

    result[decoded_k] = decoded_v;

    ++ikv_pair;
  }

  return result;
}

std::unique_ptr<eows::core::http_server> build_http_server()
{
  return std::unique_ptr<eows::core::http_server>(new eows::http::cppnetlib::http_server);
}

void
eows::http::cppnetlib::initialize()
{
  EOWS_LOG_INFO("Initializing C++ Network Library HTTP support...");

  eows::core::http_server_builder::instance().insert("cppnetlib", build_http_server);

  EOWS_LOG_INFO("C++ Network Library HTTP support initialized!");
}

