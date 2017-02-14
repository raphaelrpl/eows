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
  \file eows/core/http_server_builder.cpp

  \brief A singleton for registering builders of available HTTP server implementation.

  \author Gilberto Ribeiro de Queiroz
 */

// EOWS
#include "http_server_builder.hpp"
#include "exception.hpp"
#include "http_server.hpp"

// STL
#include <map>

// Boost
#include <boost/format.hpp>

struct eows::core::http_server_builder::impl
{
  std::map<std::string, http_server_builder_functor> builders;
};

std::unique_ptr<eows::core::http_server>
eows::core::http_server_builder::build(const std::string& builder_id)
{
  std::map<std::string, http_server_builder_functor>::const_iterator it = pimpl_->builders.find(builder_id);
  
  if(it == pimpl_->builders.end())
  {
    boost::format err_msg("Could not find an http server builder: %1%.");

    throw std::out_of_range((err_msg % builder_id).str());
  }
  
  std::unique_ptr<http_server> retval = it->second();

  return retval;
}

void
eows::core::http_server_builder::insert(const std::string& builder_id,
                                        http_server_builder_functor builder_fnct)
{
  if(exists(builder_id))
  {
    boost::format err_msg("There is already a web server builder registered with the informed id: %1%.");
    
    throw std::invalid_argument((err_msg % builder_id).str());
  }
  
  pimpl_->builders.insert(std::make_pair(builder_id, builder_fnct));
}

void
eows::core::http_server_builder::remove(const std::string& builder_id)
{
  std::map<std::string, http_server_builder_functor>::iterator it = pimpl_->builders.find(builder_id);
  
  if(it == pimpl_->builders.end())
  {
    boost::format err_msg("Could not find web server builder: %1%.");
    
    throw std::out_of_range((err_msg % builder_id).str());
  }
  
  pimpl_->builders.erase(it);
}

eows::core::http_server_builder&
eows::core::http_server_builder::instance()
{
  static http_server_builder sbuilder;

  return sbuilder;
}

eows::core::http_server_builder::http_server_builder()
  : pimpl_(new impl)
{
}

eows::core::http_server_builder::~http_server_builder()
{
  delete pimpl_;
}

bool
eows::core::http_server_builder::exists(const std::string& builder_id) const
{
  return pimpl_->builders.find(builder_id) != pimpl_->builders.end();
}

