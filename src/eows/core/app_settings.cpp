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
  \file eows/core/app_settings.cpp

  \brief A singleton that stores the application settings.

  \author Gilberto Ribeiro de Queiroz
  \author Raphael Willian da Costa
 */

// EOWS
#include "app_settings.hpp"

// STL
#include <map>

// Boost
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

struct eows::core::app_settings::impl
{
  std::string base_dir;
  std::string tmp_data_dir;
  rapidjson::Document settings;
};

void
eows::core::app_settings::set_base_dir(const std::string& dir)
{
  boost::filesystem::path bdir(dir);
  
  if(!boost::filesystem::exists(bdir))
  {
    boost::format err_msg("Invalid base directory for EOWS: %1%.");

    throw std::invalid_argument((err_msg % dir).str());
  }
  
  if(!boost::filesystem::is_directory(bdir))
  {
    boost::format err_msg("The informed path is not a valid a base directory: %1%.");

    throw std::invalid_argument((err_msg % dir).str());
  }
  
  pimpl_->base_dir = dir;
}

void eows::core::app_settings::set_tmp_data_dir(const std::string& dir)
{
  boost::filesystem::path tmp_dir(dir);

  if (!boost::filesystem::exists(tmp_dir))
  {
    boost::system::error_code err_code;
    if (!boost::filesystem::create_directory(tmp_dir, err_code))
    {
      boost::format err_msg("Error while creating temporary directory \"" + dir + "\": %1%");

      if (err_code.value() != boost::system::errc::file_exists)
      {
        switch(err_code.value())
        {
          case boost::system::errc::permission_denied:
            err_msg = err_msg % "Permission denied";
            break;
          default:
            err_msg = err_msg % "Unknown error";
        }
        throw std::invalid_argument(err_msg.str());
      }
    }
  }
  else
  {
    if(!boost::filesystem::is_directory(tmp_dir))
    {
      boost::format err_msg("The informed path is not a valid directory: %1%");

      throw std::invalid_argument((err_msg % dir).str());
    }
  }

  pimpl_->tmp_data_dir = dir;
}

const std::string&
eows::core::app_settings::get_base_dir() const
{
  return pimpl_->base_dir;
}

const std::string&eows::core::app_settings::get_tmp_data_dir() const
{
  return pimpl_->tmp_data_dir;
}

void
eows::core::app_settings::set(rapidjson::Document doc)
{
  pimpl_->settings = std::move(doc);
}

const rapidjson::Document&
eows::core::app_settings::get() const
{
  return pimpl_->settings;
}

eows::core::app_settings&
eows::core::app_settings::instance()
{
  static app_settings s_app;
  
  return s_app;
}

eows::core::app_settings::app_settings()
  : pimpl_(new impl)
{
  
}

eows::core::app_settings::~app_settings()
{
  delete pimpl_;
}

