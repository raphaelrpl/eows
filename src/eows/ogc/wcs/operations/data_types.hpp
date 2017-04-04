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

// STL
#include <string>

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
         */
        struct BaseRequest
        {
          std::string request;
          std::string version;
          std::string service;
        };
        /**
         * @brief Represents a GetCapabilities request structure
         */
        struct GetCapabilitiesRequest : public BaseRequest { };
        /**
         * @brief Represents DescriveCoverage Request structure
         */
        struct DescribeCoverageRequest : public BaseRequest
        {
          std::string coverageId;
        };
        /**
         * @brief Represents GetCoverage request structure
         */
        struct GetCoverageRequest : public BaseRequest
        {
          std::string coverageId;
          // TODO: implement
        };
      }
    }
  }
}

#endif // __EOF_OGC_WCS_OPERATIONS_CORE_DATA_TYPES__
