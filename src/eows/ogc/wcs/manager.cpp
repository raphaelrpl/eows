// EOWS
#include "manager.hpp"
#include "../../core/app_settings.hpp"
#include "../../core/utils.hpp"
#include "../../core/defines.hpp"
#include "../../core/logger.hpp"
#include "exception.hpp"
#include "core/utils.hpp"
#include "core/data_types.hpp"

// Boost
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

struct eows::ogc::wcs::manager::impl
{
  eows::ogc::wcs::core::capabilities_t capabilities;
};

eows::ogc::wcs::manager::manager()
  : pimpl_(new impl)
{
}

eows::ogc::wcs::manager::~manager()
{
  delete pimpl_;
}

eows::ogc::wcs::manager& eows::ogc::wcs::manager::instance()
{
  static manager singleton;
  return singleton;
}

const eows::ogc::wcs::core::capabilities_t& eows::ogc::wcs::manager::capabilities() const
{
  return pimpl_->capabilities;
}

void eows::ogc::wcs::manager::initialize()
{
  boost::filesystem::path cfg_file(eows::core::app_settings::instance().get_base_dir());

  cfg_file /= EOWS_WCS_FILE;

  EOWS_LOG_INFO("Reading file '" + cfg_file.string() + "'...");

  rapidjson::Document doc = eows::core::open_json_file(cfg_file.string());

  if(!doc.HasMember("WCSCapabilities"))
    throw eows::parse_error("Please, check key 'WCSCapabilities' in file '" EOWS_WCS_FILE "'.");

  const rapidjson::Value& jcapabilities = doc["WCSCapabilities"];

  core::read(jcapabilities, pimpl_->capabilities);

  EOWS_LOG_INFO("Finished reading file '" + cfg_file.string() + "'!");
}
