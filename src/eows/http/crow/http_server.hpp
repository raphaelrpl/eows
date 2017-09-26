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
  \file eows/http/crow/http_server.hpp

  \brief HTTP server based on Crow Library.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_HTTP_CROW_HTTP_SERVER_HPP__
#define __EOWS_HTTP_CROW_HTTP_SERVER_HPP__

// EOWS
#include "../../core/http_server.hpp"

namespace eows
{
  namespace http
  {
    namespace crow
    {
      //! HTTP server based on Crow Library.
      class http_server : public eows::core::http_server
      {
        public:

          http_server() = default;

          ~http_server() = default;

          int run();
      };

    } // end namespace crow
  }   // end namespace http
}     // end namespace eows

#endif  // __EOWS_HTTP_CROW_HTTP_SERVER_HPP__
