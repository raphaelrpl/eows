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
  \file eows/scidb/connection.hpp

  \brief A class for representing database connections retrieved from connection pool.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_SCIDB_CONNECTION_HPP__
#define __EOWS_SCIDB_CONNECTION_HPP__

// EOWS
#include "connection_impl.hpp"
#include "connection_pool.hpp"

// STL
#include <memory>
#include <utility>

namespace eows
{
  namespace scidb
  {

    //! A class for representing database connections retrieved from connection pool.
    class connection
    {
      friend class connection_pool;
      
      private:
      
        struct shared_connection
        {
          connection_impl* conn;
          
          shared_connection(connection_impl* rconn)
            : conn(rconn)
          {
          }
          
          ~shared_connection()
          {
            connection_pool::instance().release(conn);
          }
        };

      public:
      
        //! Copy constructor.
        connection(const connection& other)
          : conn_(other.conn_)
        {
        }
      
        //! Move constructor.
        connection(connection&& other) noexcept
          : conn_(std::move(other.conn_))
        {
        }
      
        connection& operator=(const connection& rhs)
        {
          std::shared_ptr<shared_connection> tmp(rhs.conn_);
          
          std::swap(conn_, tmp);
          
          return *this;
        }
      
        connection& operator=(connection&& rhs)
        {
          conn_ = std::move(rhs.conn_);
          
          return *this;
        }

        //! Destructor.
        ~connection() = default;

        //! Executes a given query using AFL or AQL.
        /*!
          \exception query_execution_error Throws an exception if query can not be executed or if an error occurs.
         */
        boost::shared_ptr< ::scidb::QueryResult >
        execute(const std::string& query_str, const bool afl = true)
        {
          return conn_->conn->execute(query_str, afl);
        }

        void completed(::scidb::QueryID id)
        {
          conn_->conn->completed(id);
        }

      protected:

        //! Constructor.
        connection(connection_impl* conn)
          : conn_(new shared_connection(conn))
        {
        }

      private:

        std::shared_ptr<shared_connection> conn_;
    };

  }  // end namespace scidb
}    // end namespace tws

#endif  // __TWS_SCIDB_CONNECTION_HPP__
