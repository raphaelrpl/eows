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
  \file eows/gdal/utils.hpp

  \brief Defins EOWS GDAL utility

  \author Raphael Willian da Costa
 */

#ifndef __EOWS_GDAL_UTILS_HPP__
#define __EOWS_GDAL_UTILS_HPP__

#include "data_types.hpp"

namespace eows
{
  namespace gdal
  {
    /*!
     * \brief It initializes EOWS Gdal module
     */
    void initialize();
    /*!
     * \brief It reads an int16 type from buffer
     * \param index - Buffer index
     * \param buffer - Buffer
     * \param value - Variable to store value from buffer
     */
    void get_int16(int index, void* buffer, double* value);
    /*!
     * \brief It sets an int16 value to buffer
     * \param index - Buffer index
     * \param buffer - Buffer
     * \param value - Value to append
     */
    void set_int16(int index, void* buffer, double* value);
    /*!
     * \brief It reads an uint16 type from buffer
     * \param index - Buffer index
     * \param buffer - Buffer
     * \param value - Variable to store value from buffer
     */
    void get_uint16(int index, void* buffer, double* value);
    /*!
     * \brief It sets an uint16 value to buffer
     * \param index - Buffer index
     * \param buffer - Buffer
     * \param value - Value to append
     */
    void set_uint16(int index, void* buffer, double* value);

    /*!
     * \brief It reads an int8 type from buffer
     * \param index - Buffer index
     * \param buffer - Buffer
     * \param value - Variable to store value from buffer
     */
    void get_int8(int index, void* buffer, double* value);
    /*!
     * \brief It sets an int8 value to buffer
     * \param index - Buffer index
     * \param buffer - Buffer
     * \param value - Value to append
     */
    void set_int8(int index, void* buffer, double* value);
    /*!
     * \brief It reads an uint8 type from buffer
     * \param index - Buffer index
     * \param buffer - Buffer
     * \param value - Variable to store value from buffer
     */
    void get_uint8(int index, void* buffer, double* value);
    /*!
     * \brief It sets an uint8 value to buffer
     * \param index - Buffer index
     * \param buffer - Buffer
     * \param value - Value to append
     */
    void set_uint8(int index, void* buffer, double* value);
    /*!
     * \brief Reads an int32 type from buffer
     * \param index - Buffer index
     * \param buffer - Buffer
     * \param value - Variable to store value
     */
    void get_int32(int index, void* buffer, double* value);
    /*!
     * \brief It sets an int32 value to buffer
     * \param index - Buffer index
     * \param buffer - Buffer
     * \param value - Value to append
     */
    void set_int32(int index, void* buffer, double* value);

    /*!
     * \brief It retrieve real size of a datatype in bytes
     * \param dt Datatype
     * \return Size in bytes of a datatype
     */
    int pixel_size(datatype dt);
  }
}

#endif // __EOWS_GDAL_UTILS_HPP__
