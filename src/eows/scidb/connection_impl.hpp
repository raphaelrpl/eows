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
  \file eows/scidb/connection_impl.hpp

  \brief The type of connection managed by the connection pool.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_SCIDB_CONNECTION_IMPL_HPP__
#define __EOWS_SCIDB_CONNECTION_IMPL_HPP__

// STL
#include <cstdint>
#include <string>

// Boost
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

// SciDB
#include <SciDBAPI.h>

namespace eows
{
  namespace scidb
  {

    //! The type of connection managed by the connection pool.
    class connection_impl : public boost::noncopyable
    {
      friend class connection_pool;
      
      public:
      
        //! Destructor.
        ~connection_impl();

        //! Try to stablish the connection to the database cluster.
        /*!
         
          \exception eows::scidb::connection_open_error If the connection can not be established.
          
          \exception std::invalid_argument If address or port is invalid.
         */
        void open(const std::string& coordinator_address = "localhost",
                  const uint16_t coordinator_port = 1239);

        //! Close the connection with the database cluster.
        /*!
          \exception eows::scidb::close_connection_error If the connection can not be closed.
         */
        void close();
      
        //! Returns true if the connection is opened.
        bool is_open() const;

        //! Returns true if the connection is closed.
        bool is_closed() const;
      
        //! Executes a given query using AFL or AQL.
        /*!
          \exception query_execution_error If query can not be executed or if an error occurs.
         */
        boost::shared_ptr< ::scidb::QueryResult >
        execute(const std::string& query_str, const bool afl = true);

        void completed(::scidb::QueryID id);
      
      protected:

        //! Constructor.
        connection_impl(std::string cluster_id);
      
        //! The pool to which this connection belongs.
        const std::string& cluster_id() const
        {
          return cluster_id_;
        }

      private:
      
        void* handle_;            //!< The real SciDB connection handle.
        std::string cluster_id_;  //!< The pool to which this connection belongs.
    };

  }  // end namespace scidb
}    // end namespace eows

#endif  // __EOWS_SCIDB_CONNECTION_IMPL_HPP__

