#ifndef __EOWS_GDAL_BAND_HPP__
#define __EOWS_GDAL_BAND_HPP__

#include <cstddef>
#include <gdal_priv.h>

namespace eows
{
  namespace gdal
  {
    class dataset_geotiff;

    class band
    {
      public:
        enum type
        {
          INT8,
          INT16,
          UINT16,
          INT32
        };

        typedef void (*get_buffer_value_ptr)(int index, void* buffer, double* value);
        typedef void (*set_buffer_value_ptr)(int index, void* buffer, double* value);

        band(dataset_geotiff* parent, const std::size_t& buffer_size, type t, GDALRasterBand* gdal_band);

        virtual ~band();

        double get_value(const std::size_t& x, const std::size_t& y);

        void set_value(const std::size_t& x, const std::size_t& y, double v);

      protected:
        void get_int16(int index, double* value) const;
        void set_int16(int index, const double* value);

      private:
        std::size_t id_;
        std::size_t size_;
        std::size_t x;
        std::size_t y;
        type type_;
        get_buffer_value_ptr getter_;
        set_buffer_value_ptr setter_;
        dataset_geotiff* parent_;
        GDALRasterBand* gdal_;
        void* buffer_;
    };
  }
}

#endif //__EOWS_GDAL_BAND_HPP__
