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
  \file eows/geoarray/data_types.cpp

  \brief Type declaration and definition for GeoArrays.

  \author Gilberto Ribeiro de Queiroz
 */

// EOWS
#include "data_types.hpp"

eows::geoarray::timeline_t::timeline_t(std::vector<std::string> time_points,
                                       const dimension_t& dim)
  : time_points_(std::move(time_points)),
    temporal_dim_(dim)
{
  time_point_idx_.clear();

  std::insert_iterator< std::map<std::string, std::size_t> > iit(time_point_idx_, time_point_idx_.begin());

  std::size_t pos = 0;

  std::transform(std::begin(time_points_), std::end(time_points_),
                 iit,
                 [&pos](const std::string& time_point) { return std::make_pair(time_point, pos++); } );
}
