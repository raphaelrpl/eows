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
  \file eows/gdal/gdal.hpp

  \brief Defines GDAL models, as format supporteds

  \author Raphael Willian da Costa
 */

#ifndef __EOWS_GDAL_GDAL_HPP__
#define __EOWS_GDAL_GDAL_HPP__

// STL string
#include <string>

// Boost noncopyable
#include <boost/noncopyable.hpp>

namespace eows
{
  namespace gdal
  {
    class gdal_handler : private boost::noncopyable
    {
      public:
        enum format
        {
          TIFF,
          GEOTIFF
        };

        gdal_handler(format fmt);
        ~gdal_handler();

      private:
        struct impl;
        impl* pimpl_;
    };
  }
}

#endif // __EOWS_GDAL_EXCEPTION_HPP__
