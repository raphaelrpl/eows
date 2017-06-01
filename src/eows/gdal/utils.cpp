#include "utils.hpp"
#include "../core/logger.hpp"

#include <gdal_priv.h>

void eows::gdal::initialize()
{
  EOWS_LOG_INFO("Initializing EOWS Gdal...");
  GDALAllRegister();

  EOWS_LOG_INFO("EOWS Gdal initialized.");
}

int eows::gdal::pixel_size(eows::gdal::datatype dt)
{
  if (dt == datatype::int8)
    return sizeof(char) * 8;

  if (dt == datatype::uint8)
    return sizeof(unsigned char);

  if (dt == datatype::int16)
    return sizeof(int16_t);

  if (dt == datatype::int32)
    return sizeof(int32_t);

  if (dt == datatype::uint16)
    return sizeof(uint16_t);

  if (dt == datatype::uint32)
    return sizeof(uint32_t);

  return 1;
}

void eows::gdal::get_int8(int index, void* buffer, double* value)
{
  *value = static_cast<double>(static_cast<int8_t*>(buffer)[index]);
}

void eows::gdal::get_int16(int index, void* buffer, double* value)
{
  *value = static_cast<double>(static_cast<short*>(buffer)[index]);
}

void eows::gdal::get_uint16(int index, void* buffer, double* value)
{
  *value = static_cast<double>(static_cast<uint16_t*>(buffer)[index]);
}

void eows::gdal::get_int32(int index, void* buffer, double* value)
{
  *value = static_cast<double>(static_cast<int32_t*>(buffer)[index]);
}

void eows::gdal::get_uint8(int index, void* buffer, double* value)
{
  *value = static_cast<double>(static_cast<uint8_t*>(buffer)[index]);
}

void eows::gdal::set_uint8(int index, void* buffer, double* value)
{
  static_cast<uint8_t*>(buffer)[index] = static_cast<uint8_t>(*value);
}

void eows::gdal::set_int8(int index, void* buffer, double* value)
{
  (static_cast<int8_t*>(buffer))[index] = static_cast<int8_t>(*value);
}

void eows::gdal::set_int16(int index, void* buffer, double* value)
{
  (static_cast<short*>(buffer))[index] = (short) *value;
}

void eows::gdal::set_uint16(int index, void* buffer, double* value)
{
  (static_cast<uint16_t*>(buffer))[index] = static_cast<uint16_t>(*value);
}

void eows::gdal::set_int32(int index, void* buffer, double* value)
{
  static_cast<int32_t*>(buffer)[index] = static_cast<int32_t>(*value);
}
