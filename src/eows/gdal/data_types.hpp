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
  \file eows/gdal/data_types.hpp

  \brief Represents useful datatypes for GDAL handling

  \author Raphael Willian da Costa
 */

#ifndef __EOWS_GDAL_DATA_TYPES_HPP__
#define __EOWS_GDAL_DATA_TYPES_HPP__

#include "exception.hpp"
#include <gdal_priv.h>

namespace eows
{
  namespace gdal
  {
    /*!
     * \brief EOWS GDAL Data type representation
     */
    enum class datatype
    {
      int8,   //!< Integer 8 bits (stl int8_t)
      uint8,  //!< Unsigned Integer 8 bits (stl uint8_t)
      int16,  //!< Integer 2 bytes (stl int16_t)
      uint16, //!< Unsigned Integer 2 bytes (stl uint16_t)
      int32,  //!< Integer 4 bytes (stl int32_t)
      uint32  //!< Unsigned Integer 4 bytes (stl uint32_t)
    };

    /*!
     * \brief Defines a EOWS Raster Band property containing metadata information like dummy value, eows data type, etc.
     */
    struct property
    {
      std::size_t index; //!< Property index (same band)
      datatype dtype;    //!< Band datatype definition
      double dummy;      //!< Dummy value for band
      int width;         //!< Band width
      int height;        //!< Band height
//      int block_x;       //!< Defines block position of axis X. Used for cache handling
//      int block_y;       //!< Defines block position of axis Y. Used for cache handling

      property(const std::size_t i, datatype t)
        : index(i), dtype(t), dummy(-9999), width(0), height(0)
      {
      }

      /*!
       * \brief Retrieves a property data type from GDALDataType
       * \param dt - GDAL datatype
       * \return Band datatype
       */
      static datatype from_gdal_datatype(GDALDataType dt)
      {
        switch(dt)
        {
          case GDT_Byte:
            return datatype::int8;
          case GDT_Int16:
            return datatype::int16;
          case GDT_UInt16:
            return datatype::uint16;
          case GDT_Int32:
            return datatype::int32;
          case GDT_UInt32:
            return datatype::uint32;
          default:
          {
            throw gdal_error("Invalid GDAL datatype");
          }
        }
      }
      /*!
       * \brief Retrieves a GDALDataType from EOWS GDAL datatype
       * \throws eows::gdal::gdal_error When a datatype does not exists or not supported yet.
       * \param dt - EOWS GDAL data type
       * \return GDAL data type
       */
      static GDALDataType from_datatype(datatype dt)
      {
        switch(dt)
        {
          case datatype::int8:
            return GDT_Byte;
          case datatype::int16:
            return GDT_Int16;
          case datatype::uint16:
            return GDT_UInt16;
          case datatype::int32:
            return GDT_Int32;
          case datatype::uint32:
            return GDT_Int32;
          default:
          {
            throw gdal_error("Invalid datatype");
          }
        }
      }
    };
  } //! end namespace gdal
}   //! end namespace eows

#endif //__EOWS_GDAL_DATA_TYPES_HPP__
