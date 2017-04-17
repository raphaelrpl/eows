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
  \file eows/http/crow/http_response.hpp

  \brief A simple wrapper around Crow HTTP response class.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_HTTP_CROW_HTTP_RESPONSE_HPP__
#define __EOWS_HTTP_CROW_HTTP_RESPONSE_HPP__

// EOWS
#include "../../core/http_response.hpp"
#include "../../core/utils.hpp"

// Crow
#include <crow_all.h>

namespace eows
{
  namespace http
  {
    namespace crow
    {

      //! A simple wrapper around Crow HTTP response class.
      class http_response : public eows::core::http_response
      {
        public:

          http_response(::crow::response& res)
            : res_(res)
          {
          }

          ~http_response() = default;
      
          void set_status(status_t new_status)
          {
            res_.code = static_cast<int>(new_status);
          }
      
          void add_header(header_type_t field_name, const std::string& value)
          {
            res_.set_header(to_str(field_name), value);
          }

          void add_header(const std::string& field_name, const std::string& value)
          {
            res_.set_header(field_name, value);
          }

          void write(const char* value, const std::size_t size)
          {
            // Creating a bytearray with correct size
            std::string bytes(value, size);
            res_.write(bytes);
          }

      private:
      
        ::crow::response& res_;
    };

    } // end namespace crow
  }   // end namespace http
}     // end namespace eows

#endif  // __EOWS_HTTP_CROW_HTTP_RESPONSE_HPP__
