#include "band.hpp"
#include "utils.hpp"
#include "dataset_geotiff.hpp"

eows::gdal::band::band(eows::gdal::dataset_geotiff* parent, const std::size_t& buffer_size, type t, GDALRasterBand* gdal_band)
  : size_(buffer_size), type_(t), parent_(parent), buffer_(nullptr), gdal_(gdal_band)
{
  buffer_ = new unsigned char[buffer_size];

  switch(t)
  {
    case INT16:
      getter_ = eows::gdal::get_int16;
      setter_ = eows::gdal::set_int16;
      break;
  }
}

eows::gdal::band::~band()
{
  if (parent_->is_open())
  {
    // if writable
    gdal_->WriteBlock(x_, y_, buffer_);
  }

  unsigned char* buff = (unsigned char*)buffer_;
  delete [] buff;
}

double eows::gdal::band::get_value(const std::size_t& x, const std::size_t& y)
{
  double output;
  const std::size_t index = x * (y + 1);
  getter_(index, buffer_, &output);
  return output;
}

void eows::gdal::band::set_value(const std::size_t& x, const std::size_t& y, double v)
{
  const std::size_t index = x * (y + 1);
  setter_(index, buffer_, &v);
}
