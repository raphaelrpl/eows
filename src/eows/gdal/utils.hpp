#ifndef __EOWS_GDAL_UTILS_HPP__
#define __EOWS_GDAL_UTILS_HPP__

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

  }
}

#endif // __EOWS_GDAL_UTILS_HPP__
