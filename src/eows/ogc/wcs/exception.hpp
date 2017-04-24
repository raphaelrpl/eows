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
  \file eows/ogc/wcs/exception.hpp

  \brief Specific exception types for OGC WCS Runtime module.

  \author Gilberto Ribeiro de Queiroz
  \author Raphael Willian da Costa
 */

#ifndef __EOWS_OGC_WCS_EXCEPTION_HPP__
#define __EOWS_OGC_WCS_EXCEPTION_HPP__

// EOWS
#include "../exception.hpp"

namespace eows
{
  //! The namespace for the OGC WCS Runtime module of EOWS.
  namespace ogc
  {
    namespace wcs
    {
      /*!
       * \brief Represents an error for coverage(s) not found
       */
      struct no_such_coverage_error : public virtual ogc_error
      {
        no_such_coverage_error(const std::string& s)
          : ogc_error(s, "NoSuchCoverage")
        {
        }
      };

      /*!
       * \brief Represents a Axis error. Used while client requesting an invalid axis name or value on subset model
       */
      struct invalid_axis_error : public virtual ogc_error
      {
        invalid_axis_error(const std::string& s)
          : ogc_error(s, "InvalidAxisLabel")
        {
        }
      };

      /*!
       * \brief Represents an invalid field while client requesting rangesubset values.
       */
      struct no_such_field_error : public virtual ogc_error
      {
        no_such_field_error(const std::string& s)
          : ogc_error(s, "NoSuchField")
        {
        }
      };
    } // end namespace wcs
  }   // end namespace ogc
}     // end namespace eows

#endif  // __EOWS_OGC_WCS_EXCEPTION_HPP__
