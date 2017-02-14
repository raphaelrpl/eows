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
  \file eows/wtss/metadata_manager.cpp

  \brief A singleton for managin metadata for WTSS operations.

  \author Gilberto Ribeiro de Queiroz
 */

// EOWS
#include "metadata_manager.hpp"
#include "../core/logger.hpp"

// STL
#include <map>

// Boost
#include <boost/format.hpp>

struct eows::wtss::metadata_manager::impl
{
  std::string cv_lst;

  std::map<std::string, std::string> cv_meta;
};

const std::string&
eows::wtss::metadata_manager::get_coverage_list() const
{
  return pimpl_->cv_lst;
}

const std::string&
eows::wtss::metadata_manager::get_coverage_metadata(const std::string& name) const
{
  std::map<std::string, std::string>::const_iterator it = pimpl_->cv_meta.find(name);

  if(it == pimpl_->cv_meta.end())
  {
    boost::format err_msg("Could not find a coverage named: %1%.");

    throw std::out_of_range((err_msg % name).str());
  }

  return it->second;
}

//! Access the singleton.
eows::wtss::metadata_manager&
eows::wtss::metadata_manager::instance()
{
  static metadata_manager smeta;

  return smeta;
}
