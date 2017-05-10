#ifndef __EOWS_GDAL_DATASETGEOTIFF_HPP__
#define __EOWS_GDAL_DATASETGEOTIFF_HPP__

// EOWS Core DataSet
#include "../core/dataset.hpp"

// STL
#include <string>
#include <memory>
#include <vector>

// Boost
#include <boost/noncopyable.hpp>

namespace eows
{
  namespace gdal
  {
    class dataset_geotiff : private boost::noncopyable
    {
      public:
        dataset_geotiff(const std::string& filename, std::size_t col, std::size_t row, std::size_t bands = 1);
        ~dataset_geotiff();

        void open();
        void close();

        const std::string filename();

        void write(std::vector<double> values, const std::size_t& band);
      private:
        struct impl;
        impl* pimpl_;
    };

    typedef std::shared_ptr<dataset_geotiff> dataset_geotiff_ptr;
  }
}

#endif // __EOWS_GDAL_DATASETGEOTIFF_HPP__
