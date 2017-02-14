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
  \file eows/core/service_operations_manager.cpp

  \brief A singleton for registering service operations.

  \author Gilberto Ribeiro de Queiroz
 */

// EOWS
#include "service_operations_manager.hpp"
#include "exception.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "web_service_handler.hpp"

// Boost
#include <boost/format.hpp>

eows::core::web_service_handler*
eows::core::service_operations_manager::get(const std::string& path) const
{
  std::map<std::string, std::unique_ptr<web_service_handler> >::const_iterator it = operations_idx_.find(path);

  if(it == operations_idx_.end())
  {
    boost::format err_msg("Could not find requested service operation: %1%.");

    throw std::out_of_range((err_msg % path).str());
  }

  return it->second.get();
}

void
eows::core::service_operations_manager::insert(const std::string& path,
                                               std::unique_ptr<web_service_handler> s_op)
{
  if(exists(path))
  {
    boost::format err_msg("There is already a service operation registered with the informed path: %1%.");

    throw std::invalid_argument((err_msg % path).str());
  }

  operations_idx_.insert(std::make_pair(path, std::move(s_op)));
}

eows::core::service_operations_manager&
eows::core::service_operations_manager::instance()
{
  static service_operations_manager s_op_manager;

  return s_op_manager;
}

std::vector<std::string>
eows::core::service_operations_manager::registered_paths() const
{
  std::vector<std::string> paths;
  
  for(const auto& p : operations_idx_)
    paths.push_back(p.first);
  
  return paths;
}
