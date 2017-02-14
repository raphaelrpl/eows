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
  \file eows/http/cppnetlib/utils.hpp

  \brief Utility functions for the HTTP server based on C++ Network Library.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_HTTP_CPPNETLIB_UTILS_HPP__
#define __EOWS_HTTP_CPPNETLIB_UTILS_HPP__

namespace eows
{
  namespace http
  {
    namespace cppnetlib
    {
      //! Split a query string into its components (the query string must be hex encoded).
      /*!
        \exception eows::parse_error Throws an exception if the query string can not be parsed.
       */
      eows::core::query_string_t expand(const std::string& query_str);
      
      /*!
        \exception std::exception It may throw exceptions if it can not initialize the module.
       */
      void initialize();

    }   // end namespace cppnetlib
  }     // end namespace http
}       // end namespace eows

#endif  // __EOWS_HTTP_CPPNETLIB_UTILS_HPP__

