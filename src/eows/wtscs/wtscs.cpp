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
#include "defines.hpp"
#include "request.hpp"
#include "../core/http_response.hpp"
#include "../core/http_request.hpp"
#include "../core/logger.hpp"
#include "../core/service_operations_manager.hpp"
#include "../core/utils.hpp"
#include "../geoarray/data_types.hpp"
#include "../geoarray/geoarray_manager.hpp"
//#include "../exception.hpp"

//// C++ Standard Library
//#include <memory>
#include <iostream>


// Boost
#include <boost/format.hpp>

// RapidJSON
#include <rapidjson/rapidjson.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <cstdlib>

using namespace std;

eows::wtscs::request* pRequestList;
static void return_exception(const char* msg, eows::core::http_response& res);

void eows::wtscs::status_handler::do_get(const eows::core::http_request& req, eows::core::http_response& res)
{
  // TODO: See how to implement the status_handler methods!.

  string return_msg("\"status_handler\"");

  res.set_status(eows::core::http_response::OK);

  res.add_header(eows::core::http_response::CONTENT_TYPE, "text/plain; charset=utf-8");
  res.add_header(eows::core::http_response::ACCESS_CONTROL_ALLOW_ORIGIN, "*");
  res.write(return_msg.c_str(), return_msg.size());
}

void eows::wtscs::list_algorithms_handler::do_get(const eows::core::http_request& req, eows::core::http_response& res)
{
  vector<string> algorithms{ "twdtw", "bfast", "bfast-monitor" };

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

void eows::wtscs::describe_algorithm_handler::do_get(const eows::core::http_request& req, eows::core::http_response& res)
{
  // TODO: See how to implement the describe_algorithm_handler methods!.

  string return_msg("\"describe_algorithm_handler\"");

  res.set_status(eows::core::http_response::OK);

  res.add_header(eows::core::http_response::CONTENT_TYPE, "text/plain; charset=utf-8");
  res.add_header(eows::core::http_response::ACCESS_CONTROL_ALLOW_ORIGIN, "*");
  res.write(return_msg.c_str(), return_msg.size());
}

void eows::wtscs::run_process_handler::do_post(const eows::core::http_request& req,
                                            eows::core::http_response& res)
{

  try
  {
    // Parsing the request into a Document.
    request* oRequest = new request;

    oRequest->set_UUID(EOWS_WTSCS_NAME);
    oRequest->set_parameters(req.content());

    oRequest->check_parameters();
    if((oRequest->get_status(oRequest->get_UUID())).compare("Scheduled") == 0)
    {
      // TODO: Save the request list on disk (wtscs_request_list.json)
      // You will use the EOWS_WTSCS_DIR string

      // Write afl
      string afl = oRequest->write_afl(dynamic_cast<eows::wtscs::twdtw_input_parameters*>(oRequest->input_parameters.get()));
      system(afl.c_str());

      // It Sends the AFL request.
      // The answer is a URL wrapping the request UUID identifier.
      // Ex. http://localhost:7654/wtscs/status?UUID=123456687


      // assembly the response
      rapidjson::StringBuffer buff;
      rapidjson::Writer<rapidjson::StringBuffer> writer(buff);

      writer.StartObject();

      writer.Key("UUID", static_cast<rapidjson::SizeType>(sizeof("UUID") -1));
      writer.String((oRequest->get_UUID()).c_str(), static_cast<rapidjson::SizeType>((oRequest->get_UUID()).length()));
      writer.EndObject();

      res.set_status(eows::core::http_response::OK);

      res.add_header(eows::core::http_response::CONTENT_TYPE, "application/json; charset=utf-8");
      res.add_header(eows::core::http_response::ACCESS_CONTROL_ALLOW_ORIGIN, "*");

      res.write(buff.GetString(), buff.GetSize());
    }
     delete oRequest;
  }
  catch(const exception& e)
  {
    return_exception(e.what(), res);
  }
  catch(...)
  {
    return_exception("Unexpected error in WTSCS run_process operation.", res);
  }
}

void eows::wtscs::cancel_process_handler::do_get(const eows::core::http_request& req, eows::core::http_response& res)
{
  // TODO: See how to implement the cancel_process_handler methods!.

  string return_msg("\"cancel_process_handler\"");

  res.set_status(eows::core::http_response::OK);

  res.add_header(eows::core::http_response::CONTENT_TYPE, "text/plain; charset=utf-8");
  res.add_header(eows::core::http_response::ACCESS_CONTROL_ALLOW_ORIGIN, "*");
  res.write(return_msg.c_str(), return_msg.size());
}

void eows::wtscs::open_request_list()
{
  // TODO: Load the request list file (wtscs_request_list.json)
  // You will use the EOWS_WTSCS_DIR string to find the json file

  EOWS_LOG_INFO("request file was analized!");
}

void eows::wtscs::initialize()
{
  EOWS_LOG_INFO("Initializing WTSCS...");

  //Read UUID using WTSCS
  eows::wtscs::open_request_list();

  unique_ptr<eows::wtscs::status_handler> s_h(new eows::wtscs::status_handler);
  eows::core::service_operations_manager::instance().insert("/wtscs/status", move(s_h));

  unique_ptr<list_algorithms_handler> la_h(new list_algorithms_handler);
  eows::core::service_operations_manager::instance().insert("/wtscs/list_algorithms", move(la_h));

  unique_ptr<describe_algorithm_handler> da_h(new describe_algorithm_handler);
  eows::core::service_operations_manager::instance().insert("/wtscs/describe_algorithm", move(da_h));

  unique_ptr<run_process_handler> rp_h(new run_process_handler);
  eows::core::service_operations_manager::instance().insert("/wtscs/run_process", move(rp_h));

  unique_ptr<cancel_process_handler> cp_h(new cancel_process_handler);
  eows::core::service_operations_manager::instance().insert("/wtscs/cancel_process", move(cp_h));

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
