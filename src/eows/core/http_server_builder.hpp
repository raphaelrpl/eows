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
  \file eows/core/http_server_builder.hpp

  \brief A singleton for registering builders of available HTTP server implementation.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_CORE_HTTP_SERVER_BUILDER_HPP__
#define __EOWS_CORE_HTTP_SERVER_BUILDER_HPP__

// STL
#include <memory>
#include <string>

// Boost
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

namespace eows
{
  namespace core
  {

// Forward declaration
    class http_server;

    //! The type of builder functions and functors.
    typedef boost::function0< std::unique_ptr<http_server> > http_server_builder_functor;

    //! A singleton for registering builders of available HTTP server implementation.
    class http_server_builder : public boost::noncopyable
    {
      public:

        //! Make a new web server of informed implementation type.
        /*!
          \exception std::out_of_range Throws an exception if a builder with the given id is not found.
          
          \todo E se ele nao conseguir construir???? qual excecao lancar????
         */
        std::unique_ptr<http_server> build(const std::string& builder_id);

        //! Register a new builder of http_server.
        /*!
          \exception std::invalid_argument Throws an exception if a builder with the same id already exists.
         */
        void insert(const std::string& builder_id,
                    http_server_builder_functor builder_fnct);

        //! unregister the http server builder identified by the given id.
        /*!
          \exception std::out_of_range If a builder with the given id is not found.
         */
        void remove(const std::string& builder_id);

        //! Access the singleton.
        static http_server_builder& instance();

      private:

        http_server_builder();

        ~http_server_builder();
      
        bool exists(const std::string& builder_id) const;

      private:

        struct impl;
      
        impl* pimpl_;
    };

  }   // end namespace core
}     // end namespace eows

#endif  // __EOWS_CORE_HTTP_SERVER_BUILDER_HPP__
