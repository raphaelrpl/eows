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
  \file eows/geoarray/metadata_manager.cpp

  \brief A singleton for managing metadata about arrays.

  \author Gilberto Ribeiro de Queiroz
 */

// TWS
#include "geoarray_manager.hpp"
#include "data_types.hpp"
#include "exception.hpp"
#include "utils.hpp"

// STL
#include <iterator>
#include <map>
#include <utility>

// Boost
#include <boost/format.hpp>

struct eows::geoarray::geoarray_manager::impl
{
  std::map<std::string, geoarray_t> arrays;
};

void
eows::geoarray::geoarray_manager::insert(const geoarray_t& a)
{
  std::string idx_entry(a.cluster_id + ":" + a.name);
  
  std::map<std::string, geoarray_t>::const_iterator it = pimpl_->arrays.find(idx_entry);

  if(it != std::end(pimpl_->arrays))
  {
    boost::format err_msg("GeoArray '%1%' is already registered.");
    
    throw std::invalid_argument((err_msg % idx_entry).str());
  }

  pimpl_->arrays.insert(std::make_pair(std::move(idx_entry), a));
}

std::vector<std::string>
eows::geoarray::geoarray_manager::list_arrays() const
{
  std::vector<std::string> arrays;
  
//  std::insert_iterator< std::vector<std::string> > iit(arrays, arrays.begin());
//  
//  std::transform(pimpl_->arrays.begin(), pimpl_->arrays.end(),
//                 iit,
//                 [](const std::map<std::string, geoarray_t>::value_type& v) -> std::string
//                 { return v.first; });

  for(const auto& v : pimpl_->arrays)
  {
    arrays.push_back(v.first);
  }
  
  return arrays;
}

const eows::geoarray::geoarray_t&
eows::geoarray::geoarray_manager::get(const std::string& cluster_id,
                                      const std::string& array_name) const
{
  std::string idx_entry(cluster_id + ":" + array_name);
  
  std::map<std::string, geoarray_t>::const_iterator it = pimpl_->arrays.find(idx_entry);

  if(it == std::end(pimpl_->arrays))
  {
    boost::format err_msg("Could not find metadata for array: '%1%'.");

    throw std::invalid_argument((err_msg % idx_entry).str());
  }

  return it->second;
}

eows::geoarray::geoarray_manager&
eows::geoarray::geoarray_manager::instance()
{
  static geoarray_manager inst;

  return inst;
}

eows::geoarray::geoarray_manager::geoarray_manager()
  : pimpl_(nullptr)
{
  pimpl_ = new impl;
}

eows::geoarray::geoarray_manager::~geoarray_manager()
{
  delete pimpl_;
}
