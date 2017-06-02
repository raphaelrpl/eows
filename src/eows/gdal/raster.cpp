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
  \file eows/gdal/raster.cpp

  \brief Defines an abstract class for raster structures

  \author Raphael Willian da Costa
 */

// EOWS
#include "raster.hpp"
#include "band.hpp"
#include "exception.hpp"
#include "data_types.hpp"

// STL
#include <cassert>

eows::gdal::raster::raster()
  : bands_(), dataset_(nullptr), metadata_(nullptr)
{

}

eows::gdal::raster::~raster()
{
  close();
}

void eows::gdal::raster::open(const std::string& path, eows::gdal::raster::access_policy policy)
{
  policy_ = policy;
  GDALAccess gdal_policy = GA_ReadOnly;

  if (policy == access_policy::write || policy == access_policy::read_write)
    gdal_policy = GA_Update;

  dataset_ = static_cast<GDALDataset*>(GDALOpen(path.c_str(), gdal_policy));

  if (dataset_ == nullptr)
    throw gdal_error("Could not open a Data set");

  raster::get_bands(this, dataset_, bands_);
}

void eows::gdal::raster::create(const std::string& filename, const std::size_t& col, const std::size_t& row, const std::vector<property>& properties)
{
  // Getting gdal GeoTiff driver
  GDALDriver* driver = GetGDALDriverManager()->GetDriverByName("GTiff");
  // Setting dataset limits
  col_ = col;
  row_ = row;

  if (properties.size() == 0)
    throw gdal_error("There is no property set. Could not determine raster type.");

  // Retrieving base datatype for dataset band
  GDALDataType dataset_type = property::from_datatype(properties[0].dtype);
  // Always create raster with one band. Once created, use properties to set respective band types
  dataset_ = driver->Create(filename.c_str(), col, row, properties.size(), dataset_type, metadata_);

  if (dataset_ == nullptr)
    throw gdal_error("Could not create data set");

  policy_ = access_policy::write;

  raster::get_bands(this, dataset_, bands_);
}

void eows::gdal::raster::close()
{
  // Cleaning up bands
  for(const band* b: bands_)
    delete b;
  bands_.clear();

  if (dataset_ != nullptr)
  {
    GDALClose(static_cast<GDALDatasetH>(dataset_));
    dataset_ = nullptr;
  }
}

eows::gdal::band* eows::gdal::raster::get_band(const std::size_t& id) const
{
  assert(id <= bands_.size());
  return bands_[id];
}

void eows::gdal::raster::set_value(const std::size_t& col, const std::size_t& row, const double& value, const std::size_t id)
{
  get_band(id)->set_value(col, row, value);
}

void eows::gdal::raster::get_value(const std::size_t& col, const std::size_t& row, const std::size_t id, double* value) const
{
  get_band(id)->get_value(col, row, value);
}

void eows::gdal::raster::transform(const double llx, const double lly, const double urx, const double ury, const double resx, const double resy)
{
//  double gtransform[] {llx, resx, urx, lly, ury, resy};
  double gtransform[] = {llx, resx, 0, lly, 0, resy};

  dataset_->SetGeoTransform(&gtransform[0]);
}

void eows::gdal::raster::set_name(const std::string& name)
{
  set_metadata(metadata::document_name, name);
}

void eows::gdal::raster::set_description(const std::string& desc)
{
  set_metadata(metadata::image_description, desc);
}

void eows::gdal::raster::set_metadata(eows::gdal::raster::metadata flag, const std::string& value)
{
  metadata_ = CSLSetNameValue(metadata_, get_metadata_key(flag).c_str(), value.c_str());
}

void eows::gdal::raster::set_projection(const std::string& proj_wkt)
{
  dataset_->SetProjection(proj_wkt.c_str());
}

void eows::gdal::raster::get_bands(raster* rst, GDALDataset* dset, std::vector<band*>& bands)
{
  for(int index = 0; index < dset->GetRasterCount(); ++index)
    bands.push_back(new band(rst, index, dset->GetRasterBand(index + 1)));
}

const std::string eows::gdal::raster::get_metadata_key(eows::gdal::raster::metadata flag)
{
  switch(flag)
  {
    case metadata::document_name:
      return "TIFFTAG_DOCUMENTNAME";
    case metadata::image_description:
      return "TIFFTAG_IMAGEDESCRIPTION";
    case metadata::sofware:
      return "TIFFTAG_SOFTWARE";
    case metadata::datetime:
      return "TIFFTAG_DATETIME";
    case metadata::artist:
      return "TIFFTAG_ARTIST";
    case metadata::host_coputer:
      return "TIFFTAG_HOSTCOMPUTER";
    case metadata::copyright:
      return "TIFFTAG_COPYRIGHT";
    case metadata::x_resolution:
      return "TIFFTAG_XRESOLUTION";
    case metadata::y_resolution:
      return "TIFFTAG_YRESOLUTION";
    case metadata::resolution_unit:
      return "TIFFTAG_RESOLUTIONUNIT";
  }
  return "";
}
