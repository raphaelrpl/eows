#ifndef __EOWS_GDAL_UTILS_HPP__
#define __EOWS_GDAL_UTILS_HPP__

#include "data_types.hpp"

namespace eows
{
  namespace gdal
  {
    /*!
     * \brief It initializes EOWS Gdal module
     */
    void initialize();

    void get_int16(int index, void* buffer, double* value);

    void set_int16(int index, void* buffer, double* value);

    void get_uint16(int index, void* buffer, double* value);

    void set_uint16(int index, void* buffer, double* value);

    void get_int8(int index, void* buffer, double* value);

    void set_int8(int index, void* buffer, double* value);

    int pixel_size(datatype dt);
  }
}

#endif // __EOWS_GDAL_UTILS_HPP__
