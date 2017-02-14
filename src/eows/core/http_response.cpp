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
  \file eows/core/http_response.cpp

  \brief Base class that models an HTTP response.

  \author Gilberto Ribeiro de Queiroz
 */

// EOWS
#include "http_response.hpp"

const char eows::core::http_response::access_control_allow_origin_[] = "Access-Control-Allow-Origin";
const char eows::core::http_response::content_length_[] = "Content-Length";
const char eows::core::http_response::content_type_[] = "Content-Type";
const char eows::core::http_response::unknown_[] = "Unknown";
