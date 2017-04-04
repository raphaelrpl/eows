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
  \file eows/ogc/exception.hpp

  \brief Specific exception types for OGC Runtime module.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_OGC_EXCEPTION_HPP__
#define __EOWS_OGC_EXCEPTION_HPP__

// EOWS
#include "../exception.hpp"

namespace eows
{
  //! The namespace for the OGC Runtime module of EOWS.
  namespace ogc
  {
    /**
     * @brief Generic error for OGC service
     */
    struct ogc_error : public eows_error
    {
      explicit ogc_error(const std::string& s) : eows_error(s) {}
      explicit ogc_error(const char*& s) : eows_error(s) {}
    };

    /**
     * @brief Used when user misses required parameter(s) on OGC service.
     */
    struct missing_parameter_error : public virtual ogc_error
    {
      explicit missing_parameter_error(const std::string& s) : ogc_error(s) { }
      explicit missing_parameter_error(const char* s) : ogc_error(s) { }
    };

    /**
     * @brief Used when a function or operation is not implemented on OGC service yet.
     */
    struct not_implemented_error : public virtual ogc_error
    {
      explicit not_implemented_error(const std::string& s) : ogc_error(s) { }
      explicit not_implemented_error(const char* s) : ogc_error(s) { }
    };
  }  // end namespace ogc
}    // end namespace eows

#endif  // __EOWS_OGC_EXCEPTION_HPP__
