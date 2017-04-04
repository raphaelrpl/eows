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
  \file eows/ogc/wcs/operations/factory.cpp

  \brief Defines implementation of WCS factory operation builder

  \author Raphael Willian da Costa
 */

// EOWS WCS
#include "factory.hpp"
#include "../core/operation.hpp"
#include "../core/utils.hpp"
#include "../exception.hpp"
// WCS Operation Data Types
#include "data_types.hpp"
#include "get_capabilities.hpp"

std::unique_ptr<eows::ogc::wcs::core::operation> eows::ogc::wcs::operations::build_operation(const eows::core::query_string_t& query)
{
  eows::core::query_string_t::const_iterator request_it = query.find("request");

  if (request_it == query.end()) {
    throw eows::ogc::missing_parameter_error("Missing parameter 'request'");
  }

  eows::core::query_string_t::const_iterator it = query.find("service");

  if (it == query.end()) {
    throw eows::ogc::missing_parameter_error("Missing parameter 'service'");
  }

  std::unique_ptr<eows::ogc::wcs::core::operation> op;

  if (eows::ogc::wcs::core::to_lower(request_it->second) == "getcapabilities")
  {
    GetCapabilitiesRequest capabilities_request;
    op.reset(new get_capabilities());
    return std::move(op);
  }
  else if (eows::ogc::wcs::core::to_lower(request_it->second) == "describecoverage")
  {
    throw eows::ogc::not_implemented_error("Describe Coverage is not implemented yet");
  }
  else //if (request_it->second == "GetCoverage")
  {
    throw eows::ogc::not_implemented_error("Not implemented yet");
  }
}
