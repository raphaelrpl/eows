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
  \file eows/core/http_request.hpp

  \brief Base class that models an HTTP request.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_CORE_HTTP_REQUEST_HPP__
#define __EOWS_CORE_HTTP_REQUEST_HPP__

// EOWS
#include "data_types.hpp"

// STL
#include <map>
#include <string>

// Boost
#include <boost/noncopyable.hpp>

namespace eows
{
  namespace core
  {

    //! Base class that models an HTTP request.
    class http_request : public boost::noncopyable
    {
      public:

        //! Constructor.
        http_request() = default;

        //! Virtual destructor.
        virtual ~http_request() = default;

        //! The request method: GET, POST, HEAD, PUT or DELETE.
        virtual std::string method() const = 0;

        //! Requested HTTP version: 1.0 or 1.1.
        virtual std::string version() const = 0;
      
        //! The base URI (already decoded) without the query string part.
        virtual std::string path() const = 0;

        //! The query string part in the URI (the URI part after '?' not including '?'). May be NULL.
        virtual query_string_t query_string() const = 0;

        //! The request content.
        virtual const char* content() const = 0;

        //! The request data content. Must be parsed as query-string-like
        virtual query_string_t data() const = 0;

        //! The request cookies entries.
        virtual std::map<std::string, std::string> cookies() const = 0;

        //! The request content length.
        virtual std::size_t content_length() const = 0;

        //! Get a specific header field inside the request.
        virtual std::map<std::string, std::string> headers() const = 0;
    };

  }   // end namespace core
}     // end namespace eows

#endif  // __EOWS_CORE_HTTP_REQUEST_HPP__
