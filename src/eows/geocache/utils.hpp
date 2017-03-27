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
  \file eows/geocache/utils.hpp

  \brief General utility functions.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_GEOCACHE_UTILS_HPP__
#define __EOWS_GEOCACHE_UTILS_HPP__


namespace eows
{
  namespace geocache
  {

    /*!
      \exception std::exception May throw exceptions during initialization.
     */
    void initialize();

  }  // end namespace geocache
}    // end namespace eows

#endif  // __EOWS_GEOCACHE_UTILS_HPP__

