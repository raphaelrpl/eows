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
  \file eows/http/cppnetlib/http_request.hpp

  \brief A simple wrapper around C++ Network Library HTTP request class.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_HTTP_CPPNETLIB_HTTP_REQUEST_HPP__
#define __EOWS_HTTP_CPPNETLIB_HTTP_REQUEST_HPP__

// EOWS
#include "../../core/http_request.hpp"
#include "../../core/utils.hpp"

// C++ Network Library
#include <boost/network/include/http/server.hpp>

namespace eows
{
  namespace http
  {
    namespace cppnetlib
    {
      struct handler_t;

      typedef boost::network::http::server<handler_t> server_t;

      //! A simple wrapper around C++ Network Library HTTP request class.
      class http_request : public eows::core::http_request
      {
        public:

          http_request(const server_t::request& req)
            : req_(req)
          {
            path_query_str_ = eows::core::split_path_and_query_str(req_.destination);
          }

          ~http_request() = default;

          std::string method() const
          {
            return req_.method;
          }

          std::string version() const
          {
            return std::to_string(req_.http_version_major) + "." + std::to_string(req_.http_version_minor);
          }

          std::string path() const
          {
            return path_query_str_.first;
          }
      
          eows::core::query_string_t query_string() const
          {
            return eows::core::expand(path_query_str_.second);
          }

          const char* content() const
          {
            return req_.body.c_str();
          }

          std::size_t content_length() const
          {
            return req_.body.size();
          }
 
          std::map<std::string, std::string> headers() const
          {
            std::map<std::string, std::string> headers;
  
            for(const auto& m : req_.headers)
              headers.insert(std::make_pair(m.name, m.value));

            return headers;
          }
      
        private:
      
          const server_t::request& req_;
          std::pair<std::string, std::string> path_query_str_;
      };
      
    } // end namespace cppnetlib
  }   // end namespace http
}     // end namespace eows

#endif  // __EOWS_HTTP_CPPNETLIB_HTTP_REQUEST_HPP__
