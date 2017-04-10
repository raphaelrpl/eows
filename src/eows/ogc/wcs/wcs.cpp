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
  \file eows/ogc/wcs/wcs.cpp

  \brief OGC Web Coverage Service.

  \author Gilberto Ribeiro de Queiroz
 */

// EOWS
#include "wcs.hpp"
#include "../../core/http_response.hpp"
#include "../../core/http_request.hpp"
#include "../../core/logger.hpp"
#include "../../core/service_operations_manager.hpp"
#include "../../core/utils.hpp"
#include "manager.hpp"
// WCS Operations
#include "operations/factory.hpp"
#include "operations/error_handler.hpp"
// STL
#include <sstream>
#include <memory>

//! It prepares a response output
void make_response_error(eows::core::http_response& response, const std::string& error_msg, const std::string& content_type)
{
  response.set_status(eows::core::http_response::bad_request);
  response.add_header(eows::core::http_response::CONTENT_TYPE, content_type);
  response.add_header(eows::core::http_response::ACCESS_CONTROL_ALLOW_ORIGIN, "*");
  response.write(error_msg.c_str(), error_msg.size());
}

void eows::ogc::wcs::handler::do_get(const eows::core::http_request& req,
                                     eows::core::http_response& res)
{
  try
  {
    // Putting every request parameters keys to lowercase
    eows::core::query_string_t qstr = eows::core::lowerify(req.query_string());

    // Retrieve WCS Operation
    std::unique_ptr<eows::ogc::wcs::core::operation> op(operations::build_operation(qstr));

    // Executing Operation
    op->execute();

//    std::ostringstream ss;
//    for(int i = 0; i < 5000000; ++i)
//      ss << i << " ";

    // Retrieving Result
    std::string output = op->to_string();
    std::cout << output.size() << std::endl;
    // std::string output = ss.str();

    res.set_status(eows::core::http_response::OK);
    res.add_header(eows::core::http_response::CONTENT_TYPE, op->content_type());
    res.add_header(eows::core::http_response::ACCESS_CONTROL_ALLOW_ORIGIN, "*");
    res.write(output.c_str(), output.size());
  }
  catch(const eows::ogc::ogc_error& e)
  {
    // Use WCS error handler
    make_response_error(res, operations::handle_error(e), "application/xml");
  }
  catch(const std::exception& e)
  {
    make_response_error(res, e.what(), "text/plain; charset=utf-8");
  }
}

void eows::ogc::wcs::initialize()
{
  EOWS_LOG_INFO("Initializing OGC WCS...");

  manager::instance().initialize();

  std::unique_ptr<handler> h(new handler);
  eows::core::service_operations_manager::instance().insert("/wcs", std::move(h));

  EOWS_LOG_INFO("OGC WCS service initialized!");
}

