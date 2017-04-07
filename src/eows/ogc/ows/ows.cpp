#include "ows.hpp"
#include "manager.hpp"
#include "../../core/logger.hpp"

void eows::ogc::ows::initialize()
{
  EOWS_LOG_INFO("Initializing OGC OWS module");
  manager::instance().initialize();

  EOWS_LOG_INFO("OGC OWS module initialized");
}
