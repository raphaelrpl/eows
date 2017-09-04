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
  \file eows/auth/utils.hpp

  \brief Defines utility helpers for auth module

  \author Raphael Willian da Costa
 */

#ifndef __EOWS_AUTH_UTILS_HPP__
#define __EOWS_AUTH_UTILS_HPP__

// STL
#include <string>
#include <map>

namespace eows
{
  namespace auth
  {
    struct oauth_parameters;
    /*!
     * \brief Performs Authentication module initialization
     *
     * \throws std::runtime_error When could not intialize module
     */
    void initialize();

    void replace(std::string& tpl, const std::multimap<std::string, std::string>& values, const std::string& prefix = "{{", const std::string& postfix = "}}");
  }
}

#endif // __EOWS_AUTH_UTILS_HPP__
