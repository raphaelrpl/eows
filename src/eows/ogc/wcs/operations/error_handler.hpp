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
  \file eows/ogc/wcs/operations/error_handler.cpp

  \brief This class defines WCS 2.0 error handler

  \author Raphael Willian da Costa
 */

#ifndef __EOWS_OGC_WCS_OPERATIONS_ERROR_HANDLES_HPP__
#define __EOWS_OGC_WCS_OPERATIONS_ERROR_HANDLES_HPP__

// EOWS
#include "../core/operation.hpp"

#include "../../exception.hpp"

namespace eows
{
  namespace ogc
  {
    namespace wcs
    {
      namespace operations
      {
        /**
         * @brief It prepares WCS 2.0 XML error using eows ogc exception.
         * @todo Should it handle std::exception?
         *
         * @return String representation (XML) of WCS 2.0 error document
         */
        const std::string handle_error(const eows::ogc::ogc_error&);
      }
    }
  }
}

#endif // __EOWS_OGC_WCS_OPERATIONS_ERROR_HANDLES_HPP__
