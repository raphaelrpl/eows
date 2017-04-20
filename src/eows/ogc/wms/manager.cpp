/*
  Copyright (C) 2017 National Institute For Space Research (INPE) - Brazil.

  This file is part of Earth Observation Web Services (EOWS).

  EOWS is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3 as
  published by the Free Software Foundation.

  EOWS is distributed  "AS-IS" in the hope that it will be useful,
  but WITHOUT ANY WARRANTY OF ANY KIND; without even the implied warranty
  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with EOWS. See LICENSE. If not, write to
  e-sensing team at <esensing-team@dpi.inpe.br>.
 */

/*!
  \file eows/ogc/wms/manager.cpp

  \brief A singleton for managing the WMS capabilities.

  \author Gilberto Ribeiro de Queiroz
 */

// EOW
#include "manager.hpp"
#include "defines.hpp"
#include "../../core/app_settings.hpp"
#include "../../core/logger.hpp"
#include "../../core/utils.hpp"
#include "../../exception.hpp"
#include "data_types.hpp"
#include "utils.hpp"

// Boost
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

struct eows::ogc::wms::manager::impl
{
  capabilities_t capabilities;
};

const eows::ogc::wms::capabilities_t&
eows::ogc::wms::manager::get_capabilities() const
{
  return pimpl_->capabilities;
}

void eows::ogc::wms::manager::initialize()
{
  boost::filesystem::path cfg_file(eows::core::app_settings::instance().get_base_dir());

  cfg_file /= EOWS_WMS_FILE;

  EOWS_LOG_INFO("Reading file '" + cfg_file.string() + "'...");

  rapidjson::Document doc = eows::core::open_json_file(cfg_file.string());

  if(!doc.HasMember("WMS_Capabilities"))
    throw eows::parse_error("Please, check key 'WMS_Capabilities' in file '" EOWS_WMS_FILE "'.");

  const rapidjson::Value& jcapabilities = doc["WMS_Capabilities"];

  read(jcapabilities, pimpl_->capabilities);

  EOWS_LOG_INFO("Finished reading file '" + cfg_file.string() + "'!");
}

eows::ogc::wms::manager&
eows::ogc::wms::manager::instance()
{
  static manager s_man;

  return s_man;
}

eows::ogc::wms::manager::manager()
  : pimpl_(new impl)
{
}

eows::ogc::wms::manager::~manager()
{
  delete pimpl_;
}
