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
  \file eows/scidb/connection_pool.hpp

  \brief A connection pool for SciDB connections.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_SCIDB_CONNECTION_POOL_HPP__
#define __EOWS_SCIDB_CONNECTION_POOL_HPP__

// STL
#include <string>

// Boost
#include <boost/noncopyable.hpp>

namespace eows
{
  namespace scidb
  {
    // Forward declaration
    class connection;
    class connection_impl;
    
    struct cluster_info_t;

    //! A connection pool for SciDB connections.
    class connection_pool : public boost::noncopyable
    {
      friend class connection;

      public:
      
        /*!
          \exception std::invalid_argument If a pool for the cluster doesn't exist.
          
          \exception connection_pool_limit_error If the cluster connection pool has reached its limit.
          
          \exception open_connection_error If error occur when opening the connection.
         */
        connection get(const std::string& cluster_id);
      
        /*!
          \exception std::invalid_argument If a connection pool for the cluster already exists.
         */
        void add(const cluster_info_t& cluster_info);

        static connection_pool& instance();

      protected:

        connection_pool();

        ~connection_pool();
      
        /*!
          \exception std::invalid_argument If a pool for the connection cluster doesn't exist.
         */
        void release(connection_impl* conn);

      private:

        struct impl;

        impl* pimpl_;
    };

  }   // end namespace scidb
}     // end namespace eows

#endif  // __EOWS_SCIDB_CONNECTION_POOL_HPP__
