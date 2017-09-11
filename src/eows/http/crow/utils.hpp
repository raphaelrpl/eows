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
  \file eows/http/crow/utils.hpp

  \brief Utility functions for the HTTP server based on Crow Library.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_HTTP_CROW_UTILS_HPP__
#define __EOWS_HTTP_CROW_UTILS_HPP__

// EOWS
#include "../../core/data_types.hpp"

namespace eows
{
  namespace core
  {
    class http_request;
  }

  namespace http
  {
    namespace crow
    {
      //! Split a query string into its components (the query string must be hex encoded).
      /*!
        \exception eows::parse_error Throws an exception if the query string can not be parsed.
       */
      eows::core::query_string_t expand(const std::string& query_str);

      std::map<std::string, std::string> parse_cookies(const eows::core::http_request& request);

      void initialize();

    }   // end namespace crow
  }     // end namespace http
}       // end namespace eows

#endif  // __EOWS_HTTP_CROW_UTILS_HPP__
