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
  \file eows/echo/echo.cpp

  \brief A simple echo service used to test EOWS.

  \author Gilberto Ribeiro de Queiroz
 */

// EOWS
#include "echo.hpp"
#include "../core/http_response.hpp"
#include "../core/http_request.hpp"
#include "../core/logger.hpp"
#include "../core/service_operations_manager.hpp"

void eows::echo::handler::do_get(const eows::core::http_request& req,
                                 eows::core::http_response& res)
{
  eows::core::query_string_t qstr(req.query_string());

  eows::core::query_string_t::const_iterator it = qstr.find("message");

  std::string return_msg = (it != qstr.end()) ? it->second
                                              : std::string("\"Hello from Earth Observation Web Services\"");

  res.set_status(eows::core::http_response::OK);
  
  res.add_header(eows::core::http_response::CONTENT_TYPE, "text/plain; charset=utf-8");
  res.add_header(eows::core::http_response::ACCESS_CONTROL_ALLOW_ORIGIN, "*");
  
  res.write(return_msg.c_str(), return_msg.size());
}

void
eows::echo::initialize()
{
  EOWS_LOG_INFO("Initializing echo service...");

  std::unique_ptr<handler> h(new handler);
  eows::core::service_operations_manager::instance().insert("/echo", std::move(h));

  EOWS_LOG_INFO("echo service initialized!");
}

