#include "utils.hpp"
#include "../core/logger.hpp"

#include <gdal_priv.h>

void eows::gdal::initialize()
{
  EOWS_LOG_INFO("Initializing EOWS Gdal...");
  GDALAllRegister();

  EOWS_LOG_INFO("EOWS Gdal initialized.");
}

void eows::gdal::get_int16(int index, void* buffer, double* value)
{
  *value = (double)((short*)buffer)[index];
}

void eows::gdal::set_int16(int index, void* buffer, double* value)
{
  (static_cast<short*>(buffer))[index] = (short) *value;
}
