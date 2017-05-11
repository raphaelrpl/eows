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
  \file eows/gdal/exception.hpp

  \brief Defines GDAL exception types.

  \author Raphael Willian da Costa
 */

#ifndef __EOWS_GDAL_EXCEPTION_HPP__
#define __EOWS_GDAL_EXCEPTION_HPP__

#include "../exception.hpp"

namespace eows
{
  //! Namespace for handling GDAL module of EOWS
  namespace gdal
  {
    /*!
     * \brief Represents generic error for EOWS GDAL module
     */
    struct gdal_error : public virtual eows_error
    {
      gdal_error(const std::string& msg)
        : gdal_error(msg)
      {
      }
    };
  }
}

#endif // __EOWS_GDAL_EXCEPTION_HPP__
