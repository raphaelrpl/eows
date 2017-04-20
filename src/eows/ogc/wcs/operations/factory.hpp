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
  \file eows/ogc/wcs/operations/factory.hpp

  \brief Represents a factory WCS operation builder

  \author Raphael Willian da Costa
 */

#ifndef __EOWS_OGC_WCS_OPERATIONS_HPP__
#define __EOWS_OGC_WCS_OPERATIONS_HPP__

// EOWS
#include "../../../core/data_types.hpp"
// STL
#include <memory>

namespace eows
{
  namespace ogc
  {
    namespace wcs
    {
      namespace core
      {
        //! Forward declarations
        class operation;
      }

      namespace operations
      {
        /**
         * \brief It builds a OGC WCS operation based on query string
         * \throws missing_parameter_error When request does not match required arguments
         * \return Unique Ptr to the built operation
         */
        std::unique_ptr<eows::ogc::wcs::core::operation> build_operation(const eows::core::query_string_t&);
      }
    }
  }
}

#endif // __EOWS_OGC_WCS_OPERATIONS_HPP__
