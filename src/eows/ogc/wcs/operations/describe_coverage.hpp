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
  \file eows/ogc/wcs/operations/describe_coverage.hpp

  \brief Represents WCS DescribeCoverage operation that retrieves XML representation

  \author Raphael Willian da Costa
 */

#ifndef __EOWS_OGC_WCS_OPERATIONS_DESCRIBE_COVERAGE_HPP__
#define __EOWS_OGC_WCS_OPERATIONS_DESCRIBE_COVERAGE_HPP__

// EOWS
#include "../core/operation.hpp"

namespace eows
{
  namespace ogc
  {
    namespace wcs
    {
      namespace operations
      {
        struct describe_coverage_request;

        /**
         * @brief Represents WCS GetCapabilities implementation
         * It converts cached capability json document as gml+xml style
         */
        class describe_coverage : public eows::ogc::wcs::core::operation
        {
          public:
            describe_coverage(const describe_coverage_request&);
            ~describe_coverage();

            /**
             * @brief Prepares XML encoder in order to generate WCS GetCapabilities
             */
            void execute();

            /**
             * @brief For WCS GetCapabilities operations, it is fixed to "application/gml+xml"
             * @return
             */
            const char* content_type() const;

            /**
             * @brief Retrieves XML string representation of WCS GetCapabilities 2.0
             * @return
             */
            const std::string& to_string() const override;
          private:
            struct impl;
            impl* pimpl_;
        };
      }
    }
  }
}

#endif // __EOWS_OGC_WCS_OPERATIONS_DESCRIBE_COVERAGE_HPP__
