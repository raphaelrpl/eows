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
  \author Raphael Willian da Costa
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
     * \brief Generic error for OGC service
     */
    struct ogc_error : public eows_error
    {
      explicit ogc_error(const std::string& s, const std::string& c)
        : eows_error(s)
      {
        error_code = c;
      }
      explicit ogc_error(const char*& s, const std::string& c)
        : eows_error(s), error_code(c) {}

      virtual ~ogc_error() = default;

      //! Defines code error useful during exception handling
      std::string error_code;
      //! Defines value user typed during OGC request
      std::string locator;
    };

    /**
     * \brief Used when user misses required parameter(s) on OGC service.
     */
    struct missing_parameter_error : public virtual ogc_error
    {
      explicit missing_parameter_error(const std::string& s, const std::string& code) : ogc_error(s, code) { }
      explicit missing_parameter_error(const char* s, const std::string& code) : ogc_error(s, code) { }
    };

    /**
     * \brief Used when user gives parameter, but it seems inconsistent or invalid
     */
    struct invalid_parameter_error : public virtual ogc_error
    {
      explicit invalid_parameter_error(const std::string& s, const std::string& code) : ogc_error(s, code) { }
      explicit invalid_parameter_error(const char* s, const std::string& code) : ogc_error(s, code) { }
    };

    /**
     * \brief Used when a function or operation is not implemented on OGC service yet.
     */
    struct not_implemented_error : public virtual ogc_error
    {
      explicit not_implemented_error(const std::string& s, const std::string& code) : ogc_error(s, code) { }
      explicit not_implemented_error(const char* s, const std::string& code) : ogc_error(s, code) { }
    };

    /**
     * \brief Used when a function or operation is not currently supported
     */
    struct not_supported_error : public virtual ogc_error
    {
      explicit not_supported_error(const std::string& s, const std::string& code) : ogc_error(s, code) { }
      explicit not_supported_error(const char* s, const std::string& code) : ogc_error(s, code) { }
    };
  }  // end namespace ogc
}    // end namespace eows

#endif  // __EOWS_OGC_EXCEPTION_HPP__
