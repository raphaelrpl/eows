#ifndef __EOWS_GDAL_DATA_TYPES_HPP__
#define __EOWS_GDAL_DATA_TYPES_HPP__

#include "exception.hpp"
#include <gdal_priv.h>

namespace eows
{
  namespace gdal
  {
    enum class datatype
    {
      int8,
      uint8,
      int16,
      uint16,
      int32,
      uint32
    };

    struct property
    {
      std::size_t index;
      datatype dtype;
      double dummy;
      int width;
      int height;
      int block_x;
      int block_y;

      property(const std::size_t i, datatype t)
        : index(i), dtype(t)
      {
      }

      static datatype from_gdal_datatype(GDALDataType dt)
      {
        switch(dt)
        {
          case GDT_Byte:
            return datatype::int8;
          case GDT_Int16:
            return datatype::int16;
          case GDT_UInt16:
            return datatype::uint16;
          case GDT_Int32:
            return datatype::int32;
          case GDT_UInt32:
            return datatype::uint32;
          default:
          {
            throw gdal_error("Invalid GDAL datatype");
          }
        }
      }

      static GDALDataType from_datatype(datatype dt)
      {
        switch(dt)
        {
          case datatype::int8:
            return GDT_Byte;
          case datatype::int16:
            return GDT_Int16;
          case datatype::uint16:
            return GDT_UInt16;
          case datatype::int32:
            return GDT_Int32;
          case datatype::uint32:
            return GDT_Int32;
          default:
          {
            throw gdal_error("Invalid datatype");
          }
        }
      }
    };
  }
}

#endif //__EOWS_GDAL_DATA_TYPES_HPP__
