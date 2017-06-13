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
  \file eows/gdal/utils.cpp

  \brief Implementation of EOWS GDAL utility

  \author Raphael Willian da Costa
 */

#include "utils.hpp"
#include "../core/logger.hpp"
#include "../geoarray/data_types.hpp"

#include <gdal_priv.h>

void eows::gdal::initialize()
{
  EOWS_LOG_INFO("Initializing EOWS Gdal...");
  GDALAllRegister();

  EOWS_LOG_INFO("EOWS Gdal initialized.");
}

int eows::gdal::pixel_size(int dt)
{
  return eows::geoarray::datatype_t::bytes(dt);
}

void eows::gdal::get_int8(int index, void* buffer, double* value)
{
  *value = static_cast<double>(static_cast<int8_t*>(buffer)[index]);
}

void eows::gdal::get_int16(int index, void* buffer, double* value)
{
  *value = static_cast<double>((static_cast<int16_t*>(buffer))[index]);
}

void eows::gdal::get_uint16(int index, void* buffer, double* value)
{
  *value = static_cast<double>(static_cast<uint16_t*>(buffer)[index]);
}

void eows::gdal::get_int32(int index, void* buffer, double* value)
{
  *value = static_cast<double>(static_cast<int32_t*>(buffer)[index]);
}

void eows::gdal::get_uint8(int index, void* buffer, double* value)
{
  *value = static_cast<double>(static_cast<uint8_t*>(buffer)[index]);
}

void eows::gdal::set_uint8(int index, void* buffer, double* value)
{
  static_cast<uint8_t*>(buffer)[index] = static_cast<uint8_t>(*value);
}

void eows::gdal::set_int8(int index, void* buffer, double* value)
{
  (static_cast<int8_t*>(buffer))[index] = static_cast<int8_t>(*value);
}

void eows::gdal::set_int16(int index, void* buffer, double* value)
{
  (static_cast<int16_t*>(buffer))[index] = (short) *value;
}

void eows::gdal::set_uint16(int index, void* buffer, double* value)
{
  (static_cast<uint16_t*>(buffer))[index] = static_cast<uint16_t>(*value);
}

void eows::gdal::set_int32(int index, void* buffer, double* value)
{
  static_cast<int32_t*>(buffer)[index] = static_cast<int32_t>(*value);
}
