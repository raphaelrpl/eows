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

#ifndef __EOWS_OGC_WCS_OPERATIONS_CORE_DATA_TYPES__
#define __EOWS_OGC_WCS_OPERATIONS_CORE_DATA_TYPES__

// EOWS Wcs
#include "../exception.hpp"
#include "../core/data_types.hpp"
// EOWS Core
#include "../../../core/data_types.hpp"

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
        /*!
         * \brief It defines output formats supported in WCS operation
         */
        enum class format_t
        {
          APPLICATION_GML_XML, //!< application/gml+xml,
          IMAGE_TIFF //!< image/tiff
        };

        std::string to_string(const format_t fmt);
        format_t from(const std::string& fmt);

        /**
         * \brief It represents a base request structure for WCS access
         * \throws
         */
        struct base_request
        {
          base_request(const eows::core::query_string_t& query);

          virtual ~base_request();

          std::string request; //!< WCS rqeuest operation
          std::string version; //!< WCS version
          std::string service; //!< WCS service name (Fixed to "WCS")
        };
        /**
         * \brief Represents a GetCapabilities request structure
         */
        struct get_capabilities_request : public base_request
        {
          get_capabilities_request(const eows::core::query_string_t& query);
        };
        /**
         * \brief Represents DescriveCoverage Request structure
         */
        struct describe_coverage_request : public base_request
        {
          describe_coverage_request(const eows::core::query_string_t& query);
          std::vector<std::string> coverages_id; //!< Coverage Identifier List to describe
        };
        /**
         * \brief Represents GetCoverage request structure
         */
        struct get_coverage_request : public base_request
        {
          get_coverage_request(const eows::core::query_string_t& query);

          /*!
           * \brief It process client subset and performs a minor validations like "already in" and syntax validation
           * \param query - Query string
           */
          void digest_subset(const eows::core::query_string_t& query);

          std::string coverage_id; //!< Coverage Identifier
          format_t format; //!< Response format output
          std::size_t input_crs; //!< InputCRS of subsetting
          std::size_t output_crs {4326}; //!< OutputCRS of operation. Default: 4326
          std::vector<eows::ogc::wcs::core::subset_t> subsets; //!< Client subsets to retrieve coverage portion
          eows::ogc::wcs::core::range_subset_t range_subset; //!< Coverage attributes to perform slice
        };
      }
    }
  }
}

#endif // __EOWS_OGC_WCS_OPERATIONS_CORE_DATA_TYPES__
