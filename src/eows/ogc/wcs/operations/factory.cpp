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

// EOWS
#include "factory.hpp"
#include "../core/operation.hpp"
#include "../../../core/utils.hpp"
#include "../exception.hpp"
// WCS Operation Data Types
#include "data_types.hpp"
// WCS Operation to build
#include "get_capabilities.hpp"
#include "describe_coverage.hpp"
#include "get_coverage.hpp"

std::unique_ptr<eows::ogc::wcs::core::operation> eows::ogc::wcs::operations::build_operation(const eows::core::query_string_t& query)
{
  eows::core::query_string_t::const_iterator request_it = query.find("request");

  if (request_it == query.end() || request_it->second.empty()) {
    throw eows::ogc::missing_parameter_error("Missing parameter 'request'", "request");
  }

  std::unique_ptr<eows::ogc::wcs::core::operation> op;

  if (eows::core::to_lower(request_it->second) == "getcapabilities")
  {
    get_capabilities_request capabilities_request(query);
    op.reset(new get_capabilities(capabilities_request));
    return std::move(op);
  }
  else if (eows::core::to_lower(request_it->second) == "describecoverage")
  {
    describe_coverage_request request(query);
    op.reset(new describe_coverage(request));
    return std::move(op);
  }
  else if (eows::core::to_lower(request_it->second) == "getcoverage")
  {
    get_coverage_request request(query);
    op.reset(new get_coverage(request));
    return std::move(op);
  }
  throw eows::ogc::ogc_error("Invalid WCS operation", "OperationNotSupported");
}
