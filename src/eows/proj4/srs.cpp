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
  \file eows/proj4/srs.cpp

  \brief A simple wrapper for Proj.4 support.

  \author Gilberto Ribeiro de Queiroz
 */

// EOWS
#include "srs.hpp"
#include "exception.hpp"

struct eows::proj4::srs_manager::impl
{
  std::map<std::size_t, srs_description_t> srs_idx;
};

void eows::proj4::srs_manager::insert(std::size_t srid, const srs_description_t& srs)
{
  if(pimpl_->srs_idx.find(srid) != pimpl_->srs_idx.end())
  {
    boost::format err_msg("There is already a spatial reference system registered with the informed srid: %1%.");

    throw std::invalid_argument((err_msg % srid).str());
  }

  pimpl_->srs_idx.insert(std::make_pair(srid, srs));
}

const eows::proj4::srs_description_t&
eows::proj4::srs_manager::get(std::size_t srid) const
{
  std::map<std::size_t, srs_description_t>::const_iterator it = pimpl_->srs_idx.find(srid);

  if(it == pimpl_->srs_idx.end())
  {
    boost::format err_msg("Could not find requested spatial reference system definition: %1%.");

    throw std::out_of_range((err_msg % srid).str());
  }

  return it->second;
}

bool
eows::proj4::srs_manager::exists(std::size_t srid) const
{
  return pimpl_->srs_idx.find(srid) != pimpl_->srs_idx.end();
}

eows::proj4::srs_manager&
eows::proj4::srs_manager::instance()
{
  static srs_manager smanager;

  return smanager;
}

eows::proj4::srs_manager::srs_manager()
  : pimpl_(new impl)
{

}

eows::proj4::srs_manager::~srs_manager()
{
  delete pimpl_;
}

