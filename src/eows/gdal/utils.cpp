#include "utils.hpp"
#include "../core/logger.hpp"

#include <gdal_priv.h>

void eows::gdal::initialize()
{
  EOWS_LOG_INFO("Initializing EOWS Gdal...");
  GDALAllRegister();

  EOWS_LOG_INFO("EOWS Gdal initialized.");
}
