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
  \file eows/scidb/data_types.hpp

  \brief Type declaration and definition for SciDB.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_SCIDB_DATA_TYPES_HPP__
#define __EOWS_SCIDB_DATA_TYPES_HPP__

// STL
#include <cstdint>
#include <string>

namespace eows
{
  namespace scidb
  {

    struct cluster_info_t
    {
      std::string id;
      std::string coordinator_address;
      std::size_t max_connections;
      uint16_t coordinator_port;
    };
    
  } // end namespace scidb
}   // end namespace eows

#endif // __EOWS_SCIDB_DATA_TYPES_HPP__
