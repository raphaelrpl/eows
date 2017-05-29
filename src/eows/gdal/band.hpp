#ifndef __EOWS_GDAL_BAND_HPP__
#define __EOWS_GDAL_BAND_HPP__

#include <cstddef>
#include <gdal_priv.h>

namespace eows
{
  namespace gdal
  {
    struct property;
    class raster;

    class band
    {
      public:
        typedef void (*get_buffer_value_ptr)(int index, void* buffer, double* value);
        typedef void (*set_buffer_value_ptr)(int index, void* buffer, double* value);

        band(raster* parent, const std::size_t& id, GDALRasterBand* gdal_band);

        virtual ~band();

        void get_value(const std::size_t& x, const std::size_t& y, double* value);

        void set_value(const std::size_t& x, const std::size_t& y, double v);

        std::size_t block_size();
      private:
        property* make_property(GDALRasterBand* gdalband, const std::size_t index);
        void write(std::size_t col, std::size_t row);
        int place_buffer(int col, int row);
        void read(std::size_t col, std::size_t row);

      private:
        std::size_t id_;
        int x_;
        int y_;
        property* property_;
        get_buffer_value_ptr getter_;
        set_buffer_value_ptr setter_;
        raster* parent_;
        GDALDataType datatype_;
        GDALRasterBand* gdal_;
        void* buffer_;
        bool update_buffer_;

        int current_x_;                     //!< Block x position.
        int current_y_;                     //!< Block y position.
        int current_col_;                     //!< Block column position.
        int current_row_;                     //!< Block row position.
        int current_i_;                         //!< Block index.
    };
  }
}

#endif //__EOWS_GDAL_BAND_HPP__
