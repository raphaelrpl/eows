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
  \file eows/gdal/band.hpp

  \brief Represents Raster Band for handling GDALRasterBand

  \author Raphael Willian da Costa
 */

#ifndef __EOWS_GDAL_BAND_HPP__
#define __EOWS_GDAL_BAND_HPP__

#include <cstddef>
#include <gdal_priv.h>

namespace eows
{
  namespace gdal
  {
    //! Forward declarations
    struct property;
    class raster;

    class band
    {
      public:
        //!< Helper to retrieve generic value from buffer
        typedef void (*get_buffer_value_ptr)(int index, void* buffer, double* value);
        //!< Helper to set generic value into buffer
        typedef void (*set_buffer_value_ptr)(int index, void* buffer, double* value);

        band(raster* parent, const std::size_t& id, GDALRasterBand* gdal_band);

        virtual ~band();
        /*!
         * \brief It retrieves a value from specific raster position
         * \param x - value of axis X
         * \param y - - value of axis Y
         * \param value - Pointer to value to read
         */
        void get_value(const std::size_t& x, const std::size_t& y, double* value);
        /*!
         * \brief It sets a value into a specific raster position
         * \param x - value of axis X
         * \param y - value of axis Y
         * \param v - Value to append
         */
        void set_value(const std::size_t& x, const std::size_t& y, double v);
        /*!
         * \brief Retrieve the size of entire buffer based in width, height and type value.
         * \return Size of band
         */
        std::size_t block_size();
      private:
        /*!
         * \brief It creates a EOWS Band property based in GDALRasterType.
         * \param gdalband - GDAL band
         * \param index - Band index
         * \return A pointer to created property. It does not release value automatically.
         */
        property* make_property(GDALRasterBand* gdalband, const std::size_t index);
        /*!
         * \brief It tries to write the current buffer into specific raster position
         * \todo Throw exception when gdal could not write
         * \param col - value of axis X
         * \param row - value of axis Y
         */
        void write(std::size_t col, std::size_t row);
        /*!
         * \brief It sets current position of buffer and retrieve the right position next value
         * \param col - value of axis X
         * \param row - value of axis Y
         * \return Index to insert
         */
        int place_buffer(int col, int row);

      private:
        std::size_t id_; //!< Band id
        int x_;
        int y_;
        property* property_; //!< Property metadata. TODO: Fill with dummy data, etc
        get_buffer_value_ptr getter_; //!< Buffer value getter;
        set_buffer_value_ptr setter_; //!< Buffer value setter
        raster* parent_; //!< Raster containing GDAL Dataset
        GDALDataType datatype_; //!< GDAL datatype
        GDALRasterBand* gdal_; //!< GDAL band
        void* buffer_; //!< Buffer to store raw raster data
        bool update_buffer_; //!< Flag to determines if it should save

        int current_x_;   //!< Block x position.
        int current_y_;   //!< Block y position.
        int current_col_; //!< Block column position.
        int current_row_; //!< Block row position.
        int current_i_;   //!< Block index.
    };
  }
}

#endif //__EOWS_GDAL_BAND_HPP__
