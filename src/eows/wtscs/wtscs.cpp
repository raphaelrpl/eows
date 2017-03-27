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
  \file eows/wtscs/wtscs.cpp

  \brief Web Time Series Classification Service.

  \author Eduardo Llapa Rodriguez
  \author Gilberto Ribeiro de Queiroz
 */

// EOWS
#include "wtscs.hpp"
#include "../core/http_response.hpp"
#include "../core/http_request.hpp"
#include "../core/logger.hpp"
#include "../core/service_operations_manager.hpp"
#include "../core/utils.hpp"
#include "../geoarray/data_types.hpp"
#include "../exception.hpp"

// C++ Standard Library
#include <memory>

// Boost
#include <boost/format.hpp>

// RapidJSON
#include <rapidjson/rapidjson.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

static void
return_exception(const char* msg, eows::core::http_response& res);

namespace eows
{
  namespace wtscs
  {
    struct base_input_parameters
    {
      virtual ~base_input_parameters() = default;
    };

    struct twdtw_input_parameters : base_input_parameters
    {
      std::string cv_name;
      std::vector<std::string> attributes;
      eows::geoarray::spatial_extent_t roi;
      std::string start_date;
      std::string end_date;
      std::string by;
      double overlap;
      double alpha;
      double beta;
      std::string output_array_name;
    };

    struct classify_request_parameters
    {
      std::string algorithm;
      std::unique_ptr<base_input_parameters> input_parameters;
    };

  }
}

void eows::wtscs::list_algorithms_handler::do_get(const eows::core::http_request& req,
                                                  eows::core::http_response& res)
{
    std::vector<std::string> algorithms{ "twdtw", "bfast", "bfast-monitor" };

    rapidjson::StringBuffer buff;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buff);

    writer.StartObject();

    writer.Key("algorithms", static_cast<rapidjson::SizeType>(sizeof("algorithms") -1));
    eows::core::write_string_array(algorithms.begin(), algorithms.end(), writer);

    writer.EndObject();

    res.set_status(eows::core::http_response::OK);

    res.add_header(eows::core::http_response::CONTENT_TYPE, "application/json; charset=utf-8");
    res.add_header(eows::core::http_response::ACCESS_CONTROL_ALLOW_ORIGIN, "*");

    res.write(buff.GetString(), buff.GetSize());
}

void eows::wtscs::classify_handler::do_post(const eows::core::http_request& req,
                                            eows::core::http_response& res)
{
  const char* request = req.content();

  rapidjson::Document doc;

  try
  {
    doc.Parse(request);

    if(doc.HasParseError())
    {
      boost::format err_msg("Error parsing wtscs classify request: %1%.");

      throw eows::parse_error((err_msg % doc.GetParseError()).str());
    }

    if(!doc.IsObject() || doc.IsNull())
      throw eows::parse_error("Error parsing wtscs classify request: unexpected request format.");

    classify_request_parameters req_parameters;

    rapidjson::Value::ConstMemberIterator jalgorithm = doc.FindMember("algorithm");

    if((jalgorithm == doc.MemberEnd()) || (!jalgorithm->value.IsString()))
      throw eows::parse_error("Please check key 'algorithm' in WTSCS classify request.");

    req_parameters.algorithm = jalgorithm->value.GetString();

// assembly the response
    rapidjson::StringBuffer buff;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buff);

    writer.StartObject();

    writer.Key("status", static_cast<rapidjson::SizeType>(sizeof("status") -1));
    writer.String(req_parameters.algorithm.c_str(), static_cast<rapidjson::SizeType>(req_parameters.algorithm.length()));

    writer.EndObject();

    res.set_status(eows::core::http_response::OK);

    res.add_header(eows::core::http_response::CONTENT_TYPE, "application/json; charset=utf-8");
    res.add_header(eows::core::http_response::ACCESS_CONTROL_ALLOW_ORIGIN, "*");

    res.write(buff.GetString(), buff.GetSize());
  }
  catch(const std::exception& e)
  {
    return_exception(e.what(), res);
  }
  catch(...)
  {
    return_exception("Unexpected error in WTSCS classify operation.", res);
  }
}

void
eows::wtscs::initialize()
{
  EOWS_LOG_INFO("Initializing WTSCS...");

  {
    std::unique_ptr<list_algorithms_handler> h(new list_algorithms_handler);
    eows::core::service_operations_manager::instance().insert("/wtscs/list_algorithms", std::move(h));
  }

  {
    std::unique_ptr<classify_handler> h(new classify_handler);
    eows::core::service_operations_manager::instance().insert("/wtscs/classify", std::move(h));
  }

  EOWS_LOG_INFO("WTSCS service initialized!");
}

void
return_exception(const char* msg, eows::core::http_response& res)
{
  rapidjson::StringBuffer buff;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buff);

  writer.StartObject();

  writer.Key("exception", static_cast<rapidjson::SizeType>(sizeof("exception") -1));
  writer.String(msg);

  writer.EndObject();

  res.set_status(eows::core::http_response::OK);

  res.add_header(eows::core::http_response::CONTENT_TYPE, "application/json; charset=utf-8");
  res.add_header(eows::core::http_response::ACCESS_CONTROL_ALLOW_ORIGIN, "*");

  res.write(buff.GetString(), buff.GetSize());
}
