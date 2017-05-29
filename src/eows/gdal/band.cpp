#include "band.hpp"
#include "utils.hpp"
#include "raster.hpp"
#include "data_types.hpp"
#include "exception.hpp"

#include "../core/logger.hpp"

#include <memory>
#include <iostream>

eows::gdal::band::band(eows::gdal::raster* parent, const std::size_t& id, GDALRasterBand* gdal_band)
  : id_(id), parent_(parent), gdal_(gdal_band), buffer_(nullptr)
{
  datatype_ = gdal_->GetRasterDataType();

  property_ = make_property(gdal_band, id);

  buffer_ = new unsigned char[block_size()];

  switch(datatype_)
  {
    case GDT_Byte:
      getter_ = eows::gdal::get_int8;
      setter_ = eows::gdal::set_int8;
      break;
    case GDT_Int16:
      getter_ = eows::gdal::get_int16;
      setter_ = eows::gdal::set_int16;
      break;
    case GDT_UInt16:
      break;
    case GDT_Int32:
      break;
    default:
      throw eows::gdal::gdal_error("No GDAL raster band type found");
  }

  x_ = std::numeric_limits<int>::max();

  y_ = std::numeric_limits<int>::max();
}

eows::gdal::band::~band()
{
//  if (parent_->is_open())
//  {
//    // if writable
//    gdal_->WriteBlock(x_, y_, buffer_);
//  }
  CPLErr flag = gdal_->WriteBlock(x_, y_, buffer_);
  if (flag != CE_None)
    EOWS_LOG_DEBUG("Could not write block while destroying band object");

  unsigned char* buff = (unsigned char*)buffer_;
  delete [] buff;
}

void eows::gdal::band::get_value(const std::size_t& x, const std::size_t& y, double* value)
{
  current_i_ = place_buffer(x, y);

//  const std::size_t index = x * (y + 1);
  getter_(current_i_, buffer_, value);
}

void eows::gdal::band::set_value(const std::size_t& x, const std::size_t& y, double v)
{
  current_i_ = place_buffer(x, y);
  std::cout << "Set Index " << current_i_ << " - X " << x << " - Y " << y << std::endl;

//  const std::size_t index = x * (y + 1);
  setter_(current_i_, buffer_, &v);

  update_buffer_ = true;
}

std::size_t eows::gdal::band::block_size()
{
  const int blkw = property_->width;

  const int blkh = property_->height;

  const int pxlsize = pixel_size(property_->dtype);

  const int blksize = blkw * blkh * pxlsize;

  return blksize;
}

eows::gdal::property*eows::gdal::band::make_property(GDALRasterBand* gdalband, const std::size_t index)
{
  if (gdalband == nullptr)
    return nullptr;

  std::unique_ptr<property> property_ptr(new property(index, property::from_gdal_datatype(gdalband->GetRasterDataType())));

  gdalband->GetBlockSize(&property_ptr->width, &property_ptr->height);
  property_ptr->block_x= (gdalband->GetXSize() + property_ptr->width - 1) / property_ptr->width;
  property_ptr->block_y = (gdalband->GetYSize() + property_ptr->height - 1) / property_ptr->height;

  return property_ptr.release();
}

void eows::gdal::band::write(std::size_t col, std::size_t row)
{
  CPLErr flag = gdal_->WriteBlock(col, row, buffer_);

  if (flag != CE_None)
    EOWS_LOG_DEBUG("Could not write in dataset " + std::to_string(col) + " - " + std::to_string(row));

  gdal_->FlushCache();
}

int eows::gdal::band::place_buffer(int col, int row)
{
  current_x_ = col / property_->width;
  current_y_ = row / property_->height;
  current_col_ = col % property_->width;
  current_row_ = row % property_->height;

  if (current_x_ != x_ || current_y_ != y_)
  {
    if (update_buffer_)
    {
      write(col, row);

      update_buffer_ = false;
    }

    read(current_x_, current_y_);

    x_ = current_x_;

    y_ = current_y_;
  }

  // calculates and returns the value of m_i
  return (current_col_ + current_row_ * property_->width);
}

void eows::gdal::band::read(std::size_t col, std::size_t row)
{
  if (update_buffer_)
  {
    write(col, row);
    update_buffer_ = false;
    x_ = col;
    y_ = row;
  }

  CPLErr flag = gdal_->ReadBlock(col, row, buffer_);
  if (flag != CE_None)
    EOWS_LOG_ERROR("Could not read dataset block " + std::to_string(col) + " - " + "row");
}
