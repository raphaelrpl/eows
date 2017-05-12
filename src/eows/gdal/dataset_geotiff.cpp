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
  \file eows/gdal/dataset_geotiff.hpp

  \brief Implementation of a dataset_geotiff

  \author Raphael Willian da Costa
 */

// EOWS GDAL
#include "dataset_geotiff.hpp"
#include "exception.hpp"

// EOWS GeoArray
#include "../geoarray/data_types.hpp"

// GDAL
//#include <gdal_priv.h>
#include <cpl_conv.h>

//struct eows::gdal::dataset_geotiff::impl
//{
//  impl(const std::string& f, std::size_t c, std::size_t r, std::size_t b)
//    : format("GTiff"), filename(f), col(c), row(r), bands(b), driver(nullptr)
//  {
//    driver = GetGDALDriverManager()->GetDriverByName(format.c_str());

//    if (!driver)
//      throw std::runtime_error("Could not find " + format + " driver");

//    open();
//  }

//  GDALDataset* open(const std::string& name, GDALAccess access = GA_ReadOnly)
//  {
//    return (GDALDataset*)(GDALOpen(name.c_str(), access));
//  }

//  void open()
//  {
//    dset = driver->Create(filename.c_str(), col, row, bands, GDT_Float32, nullptr);
//    if (dset == nullptr)
//      throw eows::gdal::gdal_error("Could not open dataset");
//  }

//  void close()
//  {
//    if (dset != nullptr)
//    {
//      GDALClose(static_cast<GDALDatasetH>(dset));
//      dset = nullptr;
//    }
//  }

//  ~impl()
//  {
//    close();
//  }

//  GDALDataType get_data_type(const int& type);

//  std::string format;
//  std::string filename;
//  std::size_t col;
//  std::size_t row;
//  std::size_t bands;
//  GDALDriver* driver;
//  GDALDataset* dset;
//};

//GDALDataType eows::gdal::dataset_geotiff::impl::get_data_type(const int& type)
//{
//  if (type == eows::geoarray::datatype_t::int8_dt ||
//      type == eows::geoarray::datatype_t::uint8_dt ||
//      type == eows::geoarray::datatype_t::int16_dt ||
//      type == eows::geoarray::datatype_t::uint16_dt)
//    return GDT_Int16;
//  if (eows::geoarray::datatype_t::int32_dt)
//    return GDT_Int32;
//  if (eows::geoarray::datatype_t::float_dt)
//    return GDT_Float32;
//  if (eows::geoarray::datatype_t::double_dt)
//    return GDT_Float64;

//  throw eows::gdal::gdal_error("Type not supported");
//}

eows::gdal::dataset_geotiff::dataset_geotiff(const std::string& filename, std::size_t col, std::size_t row, std::size_t bands)
  : format_("GTiff"), filename_(filename), col_(col), row_(row), bands_(bands), driver_(nullptr)
{
  driver_ = GetGDALDriverManager()->GetDriverByName(format_.c_str());

  if (!driver_)
    throw std::runtime_error("Could not find " + format_ + " driver");

  open();
}

eows::gdal::dataset_geotiff::~dataset_geotiff()
{
  close();
}

void eows::gdal::dataset_geotiff::open()
{
  dset_ = driver_->Create(filename_.c_str(), col_, row_, bands_, GDT_Float32, nullptr);
  if (dset_ == nullptr)
    throw eows::gdal::gdal_error("Could not open dataset");
}

void eows::gdal::dataset_geotiff::close()
{
  if (dset_ != nullptr)
  {
    GDALClose(static_cast<GDALDatasetH>(dset_));
    dset_ = nullptr;
  }
}

void eows::gdal::dataset_geotiff::write_int16(std::vector<GInt16> values, const std::size_t& band)
{
  GDALRasterBand* raster = dset_->GetRasterBand(band);

  const CPLErr flag = raster->RasterIO(GF_Write, 0, 0, col_, row_, &values[0], col_, row_, GDT_Int16, 0, 0);

  if (flag != CE_None)
    throw eows::gdal::gdal_error("Could not write in dataset");
}

void eows::gdal::dataset_geotiff::write_int32(std::vector<GInt32> values, const std::size_t& band)
{
  GDALRasterBand* raster = dset_->GetRasterBand(band);

  const CPLErr flag = raster->RasterIO(GF_Write, 0, 0, col_, row_, &values[0], col_, row_, GDT_Int32, 0, 0);

  if (flag != CE_None)
    throw eows::gdal::gdal_error("Could not write in dataset");
}
