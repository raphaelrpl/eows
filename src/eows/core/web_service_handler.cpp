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
  \file eows/core/web_service_handler.cpp

  \brief A base class for web services.

  \author Gilberto Ribeiro de Queiroz
 */

// EOWS
#include "web_service_handler.hpp"
#include "http_response.hpp"

void
eows::core::web_service_handler::do_get(const http_request& /*req*/,
                                        http_response& res)
{
  res.set_status(http_response::NOT_SUPPORTED);
}

void
eows::core::web_service_handler::do_head(const http_request& /*req*/,
                                         http_response& res)
{
  res.set_status(http_response::NOT_SUPPORTED);
}

void
eows::core::web_service_handler::do_post(const http_request& /*req*/,
                                         http_response& res)
{
  res.set_status(http_response::NOT_SUPPORTED);
}

void
eows::core::web_service_handler::do_put(const http_request& /*req*/,
                                        http_response& res)
{
  res.set_status(http_response::NOT_SUPPORTED);
}

void
eows::core::web_service_handler::do_delete(const http_request& /*req*/,
                                           http_response& res)
{
  res.set_status(http_response::NOT_SUPPORTED);
}

void
eows::core::web_service_handler::do_options(const http_request& /*req*/,
                                            http_response& res)
{
  res.set_status(http_response::NOT_SUPPORTED);
}

void
eows::core::web_service_handler::do_trace(const http_request& /*req*/,
                                          http_response& res)
{
  res.set_status(http_response::NOT_SUPPORTED);
}

void
eows::core::web_service_handler::do_connect(const http_request& /*req*/,
                                            http_response& res)
{
  res.set_status(http_response::NOT_SUPPORTED);
}
