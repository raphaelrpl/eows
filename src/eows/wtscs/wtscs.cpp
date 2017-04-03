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

// EOWS
#include "wtscs.hpp"
#include "../core/http_response.hpp"
#include "../core/http_request.hpp"
#include "../core/logger.hpp"
#include "../core/service_operations_manager.hpp"
#include "../core/utils.hpp"
#include "../geoarray/data_types.hpp"
//#include "../exception.hpp"

//// C++ Standard Library
//#include <memory>

// Boost
#include <boost/format.hpp>

// RapidJSON
#include <rapidjson/rapidjson.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

static void return_exception(const char* msg, eows::core::http_response& res);

void eows::wtscs::status_handler::do_get(const eows::core::http_request& req, eows::core::http_response& res)
{
//  TODO: See how to implement the status_handler methods!.

  std::string return_msg("\"status_handler\"");

  res.set_status(eows::core::http_response::OK);

  res.add_header(eows::core::http_response::CONTENT_TYPE, "text/plain; charset=utf-8");
  res.add_header(eows::core::http_response::ACCESS_CONTROL_ALLOW_ORIGIN, "*");

  res.write(return_msg.c_str(), return_msg.size());
}

void eows::wtscs::list_algorithms_handler::do_get(const eows::core::http_request& req, eows::core::http_response& res)
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

//  TODO: To get and check up the input parameters.

//  If algorithm exist then extract the specific parameters.
//
//  req_parameters.algorithm = jalgorithm->value.GetString();

//  rapidjson::Value::ConstMemberIterator jCoverage = doc.FindMember("coverage");
//  if((jInputParameters == doc.MemberEnd()) || (!jInputParameters->value.IsString()))
//  throw eows::parse_error("Please check key 'coverage' in WTSCS classify request.");
//  ...
//  Other case:
//  eows::parse_error("Algortihm no found.");

//  Obs: Store the patterns file in a directory visible by SciDB.



//  TODO: Generate the  UUID identifier to create the AFL syntax.

//  It Sends the AFL request.
//  The answer is a URL wrapping the request UUID identifier.
//  Ex. http://localhost:7654/wtscs/status?UUID=123456687


//  assembly the response
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

void eows::wtscs::initialize()
{
  EOWS_LOG_INFO("Initializing WTSCS...");

  std::unique_ptr<eows::wtscs::status_handler> s_h(new eows::wtscs::status_handler);
  eows::core::service_operations_manager::instance().insert("/wtscs/status", std::move(s_h));

  std::unique_ptr<list_algorithms_handler> la_h(new list_algorithms_handler);
  eows::core::service_operations_manager::instance().insert("/wtscs/list_algorithms", std::move(la_h));

  std::unique_ptr<classify_handler> c_h(new classify_handler);
  eows::core::service_operations_manager::instance().insert("/wtscs/classify", std::move(c_h));

  EOWS_LOG_INFO("WTSCS service initialized!");
}

void return_exception(const char* msg, eows::core::http_response& res)
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
