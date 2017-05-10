#include "gdal.hpp"
#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()


struct eows::gdal::gdal_handler::impl
{
  impl()
  {
  }

  ~impl() = default;
};

eows::gdal::gdal_handler::gdal_handler(eows::gdal::gdal_handler::format fmt)
  : pimpl_(new impl)
{

}

eows::gdal::gdal_handler::~gdal_handler()
{
  delete pimpl_;
}
