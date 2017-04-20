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
  \file eows/ogc/ows/manager.cpp

  \brief This class defines the implementation of OWS meta information access

  \author Raphael Willian da Costa
 */

// EOWS
#include "manager.hpp"
#include "utils.hpp"
#include "data_types.hpp"
#include "exception.hpp"
#include "defines.hpp"
// EOWS Core
#include "../../core/app_settings.hpp"
#include "../../core/utils.hpp"
#include "../../core/logger.hpp"

// Boost
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

struct eows::ogc::ows::manager::impl
{
  eows::ogc::ows::provider_t provider;
};

eows::ogc::ows::manager::manager()
  : pimpl_(new impl)
{
}

eows::ogc::ows::manager::~manager()
{
  delete pimpl_;
}

eows::ogc::ows::manager& eows::ogc::ows::manager::instance()
{
  static manager singleton;
  return singleton;
}

const eows::ogc::ows::provider_t& eows::ogc::ows::manager::provider() const
{
  return pimpl_->provider;
}

void eows::ogc::ows::manager::initialize()
{
  boost::filesystem::path cfg_file(eows::core::app_settings::instance().get_base_dir());

  cfg_file /= EOWS_OWS_FILE;

  EOWS_LOG_INFO("Reading file '" + cfg_file.string() + "'...");

  rapidjson::Document doc = eows::core::open_json_file(cfg_file.string());

  if(!doc.HasMember("provider"))
    throw eows::parse_error("Please, check key 'provider' in file '" EOWS_OWS_FILE "'.");

  const rapidjson::Value& provider_json = doc["provider"];

  read(provider_json, pimpl_->provider);

  EOWS_LOG_INFO("Finished reading file '" + cfg_file.string() + "'!");
}
