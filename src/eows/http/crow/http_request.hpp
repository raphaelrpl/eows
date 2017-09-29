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
  \file eows/http/crow/http_request.hpp

  \brief A simple wrapper around Crow HTTP request class.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_HTTP_CROW_HTTP_REQUEST_HPP__
#define __EOWS_HTTP_CROW_HTTP_REQUEST_HPP__

// EOWS
#include "../../core/http_request.hpp"
#include "../../core/data_types.hpp"
#include "../../core/utils.hpp"

// Crow
#include <crow_all.h>

namespace eows
{
  namespace http
  {
    namespace crow
    {
      //! A simple wrapper around Crow HTTP request class.
      class http_request : public eows::core::http_request
      {
        public:

          http_request(const ::crow::request& req)
            : req_(req)
          {
            //path_query_str_ = eows::core::split_path_and_query_str(req_.destination);
          }

          ~http_request() = default;

          std::string method() const
          {
            return ::crow::method_name(req_.method);
          }

          std::string version() const
          {
            return req_.get_header_value("version");
          }

          std::string path() const
          {
            return req_.url;
          }
      
          eows::core::query_string_t query_string() const
          {
            std::string::size_type pos = req_.raw_url.find("?");

            if(pos != std::string::npos)
              return eows::core::expand(req_.raw_url.substr(pos + 1));
            else
              return eows::core::query_string_t();
          }

          const char* content() const
          {
            return req_.body.c_str();
          }

          eows::core::query_string_t data() const
          {
            return eows::core::expand(content());
          }

          std::map<std::string, std::string> cookies() const
          {
            return eows::core::parse_cookies(*this);
          }

          std::size_t content_length() const
          {
            return req_.body.size();
          }
 
          std::map<std::string, std::string> headers() const
          {
            std::map<std::string, std::string> headers;

            for(const auto& it: req_.headers)
              headers.insert(std::make_pair(it.first, it.second));
  
            return headers;
          }
      
        private:
      
          const ::crow::request& req_;
      };
      
    } // end namespace crow
  }   // end namespace http
}     // end namespace eows

#endif  // __EOWS_HTTP_CROW_HTTP_REQUEST_HPP__
