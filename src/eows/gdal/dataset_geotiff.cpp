#include "dataset_geotiff.hpp"
#include "exception.hpp"
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
      dset = driver->Create(filename.c_str(), col, row, bands, GDT_Byte, nullptr);
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

  std::string format;
  std::string filename;
  std::size_t col;
  std::size_t row;
  std::size_t bands;
  GDALDriver* driver;
  GDALDataset* dset;
};

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

}

void eows::gdal::dataset_geotiff::close()
{
  pimpl_->close();
}

void eows::gdal::dataset_geotiff::write(std::vector<double> values, const std::size_t& band)
{
  GDALRasterBand* raster = pimpl_->dset->GetRasterBand(band);

  raster->RasterIO(GF_Write, 0, 0, pimpl_->col, pimpl_->row, &values[0], pimpl_->col, pimpl_->row, GDT_Int16, 0, 0);
}
