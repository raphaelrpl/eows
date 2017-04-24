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
  \file eows/scidb/utils.hpp

  \brief General utility functions.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_SCIDB_UTILS_HPP__
#define __EOWS_SCIDB_UTILS_HPP__

// STL
#include <string>
#include <vector>

namespace eows
{
  namespace scidb
  {

    /*!
      \exception std::exception ...
     */
    void initialize();

    /*!
     * \brief It generates a between query from SciDB
     * \param array_name - Array name
     * \param min_values - An array with min values from each axis.
     * \param max_values - An array with max values from each axis.
     * \return SciDB Between query
     */
    std::string between(const std::string& array_name, const std::vector<int>& min_values, const std::vector<int>& max_values);

  }  // end namespace scidb
}    // end namespace eows

#endif  // __EOWS_SCIDB_UTILS_HPP__
