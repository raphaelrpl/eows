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
  \file eows/http/cppnetlib/http_response.hpp

  \brief A simple wrapper around C++ Network Library HTTP response class.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_HTTP_CPPNETLIB_HTTP_RESPONSE_HPP__
#define __EOWS_HTTP_CPPNETLIB_HTTP_RESPONSE_HPP__

// EOWS
#include "../../core/http_response.hpp"
#include "../../core/utils.hpp"

// C++ Network Library
#include <boost/network/include/http/server.hpp>

// STL
#include <map>
#include <string>

namespace eows
{
  namespace http
  {
    namespace cppnetlib
    {
      struct handler_t;

      typedef boost::network::http::server<handler_t> server_t;

      //! A simple wrapper around C++ Network Library HTTP response class.
      class http_response : public eows::core::http_response
      {
        public:

          http_response(const server_t::connection_ptr& conn)
            : conn_(conn)
          {
          }

          ~http_response() = default;
      
          void set_status(status_t new_status)
          {
            conn_->set_status(static_cast<server_t::connection::status_t>(new_status));
          }
      
          void add_header(header_type_t field_name, const std::string& value)
          {
            headers_.insert(std::make_pair(to_str(field_name), value));
          }

          void add_header(const std::string& field_name, const std::string& value)
          {
            headers_.insert(std::make_pair(field_name, value));
          }

          void write(const char* value, const std::size_t size)
          {
            if(!headers_.empty())
            {
              conn_->set_headers(headers_);
              headers_.clear();
            }
            
            std::string v = value;
            conn_->write(v);
          }

      private:
      
        const server_t::connection_ptr& conn_;
        std::map<std::string, std::string> headers_;
    };

    } // end namespace cppnetlib
  }   // end namespace http
}     // end namespace eows

#endif  // __EOWS_HTTP_CPPNETLIB_HTTP_RESPONSE_HPP__
