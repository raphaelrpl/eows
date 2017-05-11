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
#include <gdal_priv.h>
#include <cpl_conv.h>

struct eows::gdal::dataset_geotiff::impl
{
  impl(const std::string& f, std::size_t c, std::size_t r, std::size_t b)
    : format("GTiff"), filename(f), col(c), row(r), bands(b), driver(nullptr)
  {
    driver = GetGDALDriverManager()->GetDriverByName(format.c_str());

    if (!driver)
      throw std::runtime_error("Could not find " + format + " driver");

    try
    {
      open();
    }
    catch(...)
    {
      delete driver;
      throw;
    }
  }

  GDALDataset* open(const std::string& name, GDALAccess access = GA_ReadOnly)
  {
    return (GDALDataset*)(GDALOpen(name.c_str(), access));
  }

  void open()
  {
    dset = driver->Create(filename.c_str(), col, row, bands, GDT_Byte, nullptr);
    if (dset == nullptr)
      throw eows::gdal::gdal_error("Could not open dataset");
  }

  void close()
  {
    if (dset != nullptr)
    {
      GDALClose(static_cast<GDALDatasetH>(dset));
      dset = nullptr;
    }
  }

  ~impl()
  {
    close();

    if (driver)
      delete driver;
  }

  GDALDataType get_data_type(const int& type);

  std::string format;
  std::string filename;
  std::size_t col;
  std::size_t row;
  std::size_t bands;
  GDALDriver* driver;
  GDALDataset* dset;
};

GDALDataType eows::gdal::dataset_geotiff::impl::get_data_type(const int& type)
{
  if (type == eows::geoarray::datatype_t::int8_dt ||
      type == eows::geoarray::datatype_t::uint8_dt ||
      type == eows::geoarray::datatype_t::int16_dt ||
      type == eows::geoarray::datatype_t::uint16_dt)
    return GDT_Int16;
  if (eows::geoarray::datatype_t::int32_dt)
    return GDT_Int32;
  if (eows::geoarray::datatype_t::float_dt)
    return GDT_Float32;
  if (eows::geoarray::datatype_t::double_dt)
    return GDT_Float64;

  throw eows::gdal::gdal_error("Type not supported");
}

eows::gdal::dataset_geotiff::dataset_geotiff(const std::string& filename, std::size_t col, std::size_t row, std::size_t bands)
  : pimpl_(new impl(filename, col, row, bands))
{

}

eows::gdal::dataset_geotiff::~dataset_geotiff()
{
  delete pimpl_;
}

void eows::gdal::dataset_geotiff::open()
{
  pimpl_->open();
}

void eows::gdal::dataset_geotiff::close()
{
  pimpl_->close();
}

void eows::gdal::dataset_geotiff::write(std::vector<double> values, const std::size_t& band, const int& type)
{
  GDALRasterBand* raster = pimpl_->dset->GetRasterBand(band);

  const GDALDataType gtype = pimpl_->get_data_type(type);

  const CPLErr flag = raster->RasterIO(GF_Write, 0, 0, pimpl_->col, pimpl_->row, &values[0], pimpl_->col, pimpl_->row, gtype, 0, 0);

  if (flag != CE_None)
    throw eows::gdal::gdal_error("Could not write in dataset");
}
