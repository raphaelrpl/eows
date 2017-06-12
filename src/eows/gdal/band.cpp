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
  \file eows/gdal/band.cpp

  \brief Implementation of Raster band

  \author Raphael Willian da Costa
 */


// EOWS
#include "band.hpp"
#include "utils.hpp"
#include "raster.hpp"
#include "data_types.hpp"
#include "exception.hpp"
// EOWS Logger
#include "../core/logger.hpp"
// STL
#include <memory>

eows::gdal::band::band(eows::gdal::raster* parent, const std::size_t& id, GDALRasterBand* gdal_band)
  : id_(id), parent_(parent), gdal_(gdal_band), buffer_(nullptr)
{
  datatype_ = gdal_->GetRasterDataType();
  // Generating eows band property
  property_ = make_property(gdal_band, id);
  // Allocating buffer based in base limits and datatype
  buffer_ = new unsigned char[block_size()];

  switch(datatype_)
  {
    case GDT_Byte:
      getter_ = eows::gdal::get_uint8;
      setter_ = eows::gdal::set_uint8;
      break;
    case GDT_Int16:
      getter_ = eows::gdal::get_int16;
      setter_ = eows::gdal::set_int16;
      break;
    case GDT_UInt16:
      getter_ = eows::gdal::get_uint16;
      setter_ = eows::gdal::set_uint16;
      break;
    case GDT_Int32:
      getter_ = eows::gdal::get_int32;
      setter_ = eows::gdal::set_int32;
      break;
    default: // GDT_Unknown
      getter_ = eows::gdal::get_int8;
      setter_ = eows::gdal::set_int8;
  }

  // TODO: Remove it.
  update_buffer_ = true;

//  x_ = 0;//std::numeric_limits<int>::max();
//  y_ = 0;//std::numeric_limits<int>::max();
}

eows::gdal::band::~band()
{
  if (update_buffer_)
  {
    int x = property_->width;
    int y = property_->height;
    CPLErr flag = gdal_->RasterIO(GF_Write, 0, 0, x, y, buffer_, x, y, datatype_, 0, 0);
    gdal_->FlushCache();
    if (flag != CE_None)
      EOWS_LOG_DEBUG("Could not write block while destroying band object");
  }

  delete property_;

  unsigned char* buff = static_cast<unsigned char*>(buffer_);
  delete [] buff;
}

void eows::gdal::band::get_value(const std::size_t& x, const std::size_t& y, double* value)
{
  current_i_ = place_buffer(x, y);
  getter_(current_i_, buffer_, value);
}

void eows::gdal::band::set_value(const std::size_t& x, const std::size_t& y, double v)
{
  current_i_ = place_buffer(x, y);

  setter_(current_i_, buffer_, &v);
}

std::size_t eows::gdal::band::block_size()
{
  const int block_width = property_->width;

  const int block_height = property_->height;

  const int pxlsize = pixel_size(property_->dtype);

  const int blksize = block_width * block_height* pxlsize;

  return blksize;
}

eows::gdal::property* eows::gdal::band::make_property(GDALRasterBand* gdalband, const std::size_t index)
{
  if (gdalband == nullptr)
    return nullptr;

  std::unique_ptr<property> property_ptr(new property(index, property::from_gdal_datatype(gdalband->GetRasterDataType())));

//  gdalband->GetBlockSize(&property_ptr->width, &property_ptr->height);
  property_ptr->width = gdalband->GetXSize();
  property_ptr->height = gdalband->GetYSize();

//  property_ptr->block_x= (gdalband->GetXSize() + property_ptr->width - 1) / property_ptr->width;
//  property_ptr->block_y = (gdalband->GetYSize() + property_ptr->height - 1) / property_ptr->height;

  return property_ptr.release();
}

void eows::gdal::band::write(std::size_t col, std::size_t row)
{
  CPLErr flag = gdal_->WriteBlock(col, row, buffer_);

  if (flag != CE_None)
    EOWS_LOG_DEBUG("Could not write in dataset " + std::to_string(col) + " - " + std::to_string(row));

  gdal_->FlushCache();

  update_buffer_ = false;
}

int eows::gdal::band::place_buffer(int col, int row)
{
  current_x_ = col / property_->width;
  current_y_ = row / property_->height;
  current_col_ = col % property_->width;
  current_row_ = row % property_->height;

  return (current_col_ + current_row_ * property_->width);
}

//void eows::gdal::band::read(std::size_t col, std::size_t row)
//{
//  if (update_buffer_)
//  {
//    write(col, row);
//    x_ = col;
//    y_ = row;
//  }

//  CPLErr flag = gdal_->ReadBlock(col, row, buffer_);
//  if (flag != CE_None)
//    EOWS_LOG_ERROR("Could not read dataset block " + std::to_string(col) + " - " + "row");
//}
