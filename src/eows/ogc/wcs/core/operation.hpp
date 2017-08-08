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
  \file eows/ogc/wcs/core/operation.hpp

  \brief Represents Generic WCS operation

  \author Raphael Willian da Costa
 */

#ifndef __EOWS_OGC_WCS_CORE_OPERATIONS_HPP__
#define __EOWS_OGC_WCS_CORE_OPERATIONS_HPP__

#include <string>

namespace eows
{
  namespace ogc
  {
    namespace wcs
    {
      namespace core
      {
        /**
         * @brief Represents a generic WCS operation skeleton. Once defined, you must implement required methods in order to
         * determines execution behavior and even content type information
         */
        class operation
        {
          public:
            operation() = default;

            virtual ~operation()
            {
            }

            /**
             * @brief It performs a WCS operation execution.
             */
            virtual void execute() = 0;

            /**
             * @brief It retrieves the content type used for this operation.
             * @return
             */
            virtual const char* content_type() const = 0;

            /**
             * @brief It retrieves the raw operation output.
             * @todo Review it. For operation like GetCoverage that processes raster information, the output is not a std string
             * @returns string representation of operation
             */
            virtual const std::string& to_string() const = 0;
        };
      }
    }
  }
}

#endif // __EOWS_OGC_WCS_CORE_OPERATIONS_HPP__
