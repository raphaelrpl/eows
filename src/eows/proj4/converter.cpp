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
  \file eows/proj4/converter.cpp

  \brief Implementation of Convertion Module handler

  \author Raphael Willian da Costa
 */

// EOWS
#include "converter.hpp"
#include "srs.hpp"

thread_local eows::proj4::spatial_ref_map t_srs_idx;

struct eows::proj4::converter::impl
{
  impl()
    : source_srs(nullptr),
      target_srs(nullptr),
      source_srid(0),
      target_srid(0)
  {}

  void set_srs(eows::proj4::spatial_reference*& srs, const int& srid, int& target_srid);

  eows::proj4::spatial_reference* source_srs;
  eows::proj4::spatial_reference* target_srs;
  int source_srid;
  int target_srid;
};

inline void eows::proj4::converter::impl::set_srs(eows::proj4::spatial_reference*& srs, const int &srid, int& target_srid)
{
  eows::proj4::spatial_reference* src_srs = nullptr;

  // First of all, we must reproject client subsets to an compatible array
  eows::proj4::spatial_ref_map::const_iterator it = t_srs_idx.find(srid);

  // If it already indexed
  if(it != t_srs_idx.end())
    src_srs = it->second.get();
  else
  {
    // Retrieving GeoArray SRS (dst)
    const eows::proj4::srs_description_t& srs_desc = eows::proj4::srs_manager::instance().get(srid);
    std::unique_ptr<eows::proj4::spatial_reference> srs(new eows::proj4::spatial_reference(srs_desc.proj4_txt));

    src_srs = srs.get();
    t_srs_idx.insert(std::make_pair(srid, std::move(srs)));
  }

  srs = src_srs;
  target_srid = srid;
}

eows::proj4::converter::converter()
  : pimpl_(new impl)
{
}

eows::proj4::converter::~converter()
{
  delete pimpl_;
}

void eows::proj4::converter::set_source_srid(const int& srid)
{
  pimpl_->set_srs(pimpl_->source_srs, srid, pimpl_->source_srid);
}

void eows::proj4::converter::set_target_srid(const int& srid)
{
  pimpl_->set_srs(pimpl_->target_srs, srid, pimpl_->target_srid);
}

void eows::proj4::converter::convert(double& x, double& y) const
{
  eows::proj4::transform(*pimpl_->source_srs, *pimpl_->target_srs, x, y);
}
