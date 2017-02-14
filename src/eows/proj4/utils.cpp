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
  \file eows/proj4/utils.cpp

  \brief Utility functions for Proj.4.

  \author Gilberto Ribeiro de Queiroz
 */

// EOWS
#include "utils.hpp"
#include "../core/app_settings.hpp"
#include "../core/defines.hpp"
#include "../core/logger.hpp"
#include "../core/utils.hpp"
#include "exception.hpp"
#include "srs.hpp"

// Boost
#include <boost/filesystem.hpp>

void eows::proj4::initialize()
{
  boost::filesystem::path cfg_file(eows::core::app_settings::instance().get_base_dir());

  cfg_file /= EOWS_SRS_FILE;

  EOWS_LOG_INFO("Reading file '" + cfg_file.string() + "'...");

  rapidjson::Document doc = eows::core::open_json_file(cfg_file.string());

  rapidjson::Value::ConstMemberIterator jsrs = doc.FindMember("spatial_reference_systems");

  if((jsrs == doc.MemberEnd()) || (!jsrs->value.IsArray()))
    throw eows::parse_error("Please check key 'spatial_reference_systems' in file: \"" EOWS_SRS_FILE "\".");

  for(rapidjson::SizeType i = 0; i < jsrs->value.Size(); ++i)
  {
    srs_description_t srs;

    const rapidjson::Value& jcrs = jsrs->value[i];

    rapidjson::Value::ConstMemberIterator jsrid = jcrs.FindMember("srid");

    if((jsrid == jcrs.MemberEnd()) || (!jsrid->value.IsUint()))
      throw eows::parse_error("Please check key 'spatial_reference_systems' in file: \"" EOWS_SRS_FILE "\".");

    std::size_t srid = jsrid->value.GetUint();

    rapidjson::Value::ConstMemberIterator jproj = jcrs.FindMember("proj4");

    if((jproj == jcrs.MemberEnd()) || (!jproj->value.IsString()))
      throw eows::parse_error("Please check key 'spatial_reference_systems' in file: \"" EOWS_SRS_FILE "\".");

    srs.proj4_txt = jproj->value.GetString();

    rapidjson::Value::ConstMemberIterator jwkt = jcrs.FindMember("wkt");

    if((jwkt == jcrs.MemberEnd()) || (!jwkt->value.IsString()))
      throw eows::parse_error("Please check key 'spatial_reference_systems' in file: \"" EOWS_SRS_FILE "\".");

    srs.wkt = jwkt->value.GetString();

    srs_manager::instance().insert(srid, srs);
  }

  EOWS_LOG_INFO("Fineshed reading file '" + cfg_file.string() + "'.");
}

