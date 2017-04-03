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
    struct ogc_error : virtual eows_error { };

    struct missing_parameter_error : virtual std::runtime_error {
      explicit missing_parameter_error(const std::string& s) : std::runtime_error(s) {}
    };

    struct not_implemented_error : virtual std::runtime_error {
      explicit not_implemented_error(const std::string& s) : std::runtime_error(s) {}
    };
  }  // end namespace ogc
}    // end namespace eows

#endif  // __EOWS_OGC_EXCEPTION_HPP__
