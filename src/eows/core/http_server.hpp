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
  \file eows/app_server/http_server.hpp

  \brief Base class for HTTP servers.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_CORE_HTTP_SERVER_HPP__
#define __EOWS_CORE_HTTP_SERVER_HPP__

// Boost
#include <boost/noncopyable.hpp>

namespace eows
{
  namespace core
  {

    //! Base class for HTTP servers.
    class http_server : public boost::noncopyable
    {
      public:

        //! Constructor.
        http_server() = default;
      
        //! Virtual destructor.
        virtual ~http_server() = default;

        /*!
          \brief Start the HTTP server.

          \return HTTP server termination status: EXIT_SUCCESS or EXIT_FAILURE.
          
          \excpetion std::exception If unexpected exception occurs.
         */
        virtual int run() = 0;
    };

  }   // end namespace core
}     // end namespace eows

#endif  // __EOWS_CORE_HTTP_SERVER_HPP__
