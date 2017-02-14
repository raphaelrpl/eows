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

void eows::ogc::wcs::handler::do_get(const eows::core::http_request& req,
                                     eows::core::http_response& res)
{
  eows::core::query_string_t qstr(req.query_string());

  std::string return_msg = qstr.empty() ? std::string("\"Hello from OGC WCS\"")
                                        : qstr.at("message");

  res.set_status(eows::core::http_response::OK);
  
  res.add_header(eows::core::http_response::CONTENT_TYPE, "text/plain; charset=utf-8");
  res.add_header(eows::core::http_response::ACCESS_CONTROL_ALLOW_ORIGIN, "*");
  
  res.write(return_msg.c_str(), return_msg.size());
}

void
eows::ogc::wcs::initialize()
{
  EOWS_LOG_INFO("Initializing OGC WCS...");

  std::unique_ptr<handler> h(new handler);
  eows::core::service_operations_manager::instance().insert("/wcs", std::move(h));

  EOWS_LOG_INFO("OGC WCS service initialized!");
}

