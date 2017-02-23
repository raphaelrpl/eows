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


void eows::wtscs::status_handler::do_get(const eows::core::http_request& req, eows::core::http_response& res)
{
  // TODO: see how to implement the status_handler methods!.

  std::string return_msg("\"status_handler\"");

  res.set_status(eows::core::http_response::OK);

  res.add_header(eows::core::http_response::CONTENT_TYPE, "text/plain; charset=utf-8");
  res.add_header(eows::core::http_response::ACCESS_CONTROL_ALLOW_ORIGIN, "*");

  res.write(return_msg.c_str(), return_msg.size());
}

void eows::wtscs::list_algorithms_handler::do_get(const eows::core::http_request& req, eows::core::http_response& res)
{
  // TODO: see how to implement the list_algorithms_handler methods!.

  std::string return_msg("\"list_algorithms_handler\"");

  res.set_status(eows::core::http_response::OK);

  res.add_header(eows::core::http_response::CONTENT_TYPE, "text/plain; charset=utf-8");
  res.add_header(eows::core::http_response::ACCESS_CONTROL_ALLOW_ORIGIN, "*");

  res.write(return_msg.c_str(), return_msg.size());
}

void eows::wtscs::classify_handler::do_get(const eows::core::http_request& req, eows::core::http_response& res)
{
  // TODO: see how to implement the classify_handler methods!.

  std::string return_msg("\"classify_handler\"");

  res.set_status(eows::core::http_response::OK);

  res.add_header(eows::core::http_response::CONTENT_TYPE, "text/plain; charset=utf-8");
  res.add_header(eows::core::http_response::ACCESS_CONTROL_ALLOW_ORIGIN, "*");

  res.write(return_msg.c_str(), return_msg.size());
}

void eows::wtscs::initialize()
{
  EOWS_LOG_INFO("Initializing WTSCS...");

  std::unique_ptr<eows::wtscs::status_handler> s_h(new eows::wtscs::status_handler);
  eows::core::service_operations_manager::instance().insert("/wtscs/status", std::move(s_h));

  std::unique_ptr<eows::wtscs::list_algorithms_handler> la_h(new eows::wtscs::list_algorithms_handler);
  eows::core::service_operations_manager::instance().insert("/wtscs/list_algorithms", std::move(la_h));

  std::unique_ptr<eows::wtscs::classify_handler> c_h(new eows::wtscs::classify_handler);
  eows::core::service_operations_manager::instance().insert("/wtscs/classify", std::move(c_h));

  EOWS_LOG_INFO("WTSCS service initialized!");
}
