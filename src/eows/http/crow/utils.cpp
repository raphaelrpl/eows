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

  \brief Utility functions for the HTTP server based on Crow Library.

  \author Gilberto Ribeiro de Queiroz
 */

// EOWS
#include "utils.hpp"
#include "../../exception.hpp"
#include "../../core/http_server_builder.hpp"
#include "../../core/logger.hpp"
#include "../../core/utils.hpp"
#include "../../core/http_request.hpp"
#include "http_server.hpp"

std::unique_ptr<eows::core::http_server> build_http_server()
{
  return std::unique_ptr<eows::core::http_server>(new eows::http::crow::http_server);
}

void
eows::http::crow::initialize()
{
  EOWS_LOG_INFO("Initializing Crow HTTP support...");

  eows::core::http_server_builder::instance().insert("crow", build_http_server);
  
  EOWS_LOG_INFO("Crow support initialized!");
}
