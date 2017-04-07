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
  \file eows/ogc/wcs/operations/data_types.hpp

  \brief Represents a OGC WCS operations data types like WCS Request Parameters

  \author Raphael Willian da Costa
 */

#ifndef __EOF_OGC_WCS_OPERATIONS_CORE_DATA_TYPES__
#define __EOF_OGC_WCS_OPERATIONS_CORE_DATA_TYPES__

#include "../../../core/data_types.hpp"
#include "../exception.hpp"
// STL
#include <string>
#include <vector>

namespace eows
{
  namespace ogc
  {
    namespace wcs
    {
      namespace operations
      {
        /**
         * @brief It represents a base request structure for WCS access
         * @throws
         */
        struct base_request
        {
          base_request(const eows::core::query_string_t& query);

          virtual ~base_request();

          virtual ~base_request() {}

          std::string request;
          std::string version;
          std::string service;
        };
        /**
         * @brief Represents a GetCapabilities request structure
         */
        struct get_capabilities_request : public base_request
        {
          get_capabilities_request(const eows::core::query_string_t& query);
        };
        /**
         * @brief Represents DescriveCoverage Request structure
         */
        struct describe_coverage_request : public base_request
        {
          describe_coverage_request(const eows::core::query_string_t& query);
          std::vector<std::string> coverages_id;
        };
        /**
         * @brief Represents GetCoverage request structure
         */
        struct get_coverage_request : public base_request
        {
          get_coverage_request(const eows::core::query_string_t& query);

          std::string coverage_id;
          std::string format;
          // TODO: implement
        };
      }
    }
  }
}

#endif // __EOF_OGC_WCS_OPERATIONS_CORE_DATA_TYPES__
