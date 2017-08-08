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
  \file eows/scidb/scoped_query.hpp

  \brief A Scoped Query handler for SciDB query results

  \author Raphael Willian da Costa
 */

#ifndef __EOWS_SCIDB_SCOPED_QUERY_HPP__
#define __EOWS_SCIDB_SCOPED_QUERY_HPP__

// EOWS Connection
#include "connection.hpp"

// EOWS Logger
#include "../core/logger.hpp"

// SciDB
#include <SciDBAPI.h>

// Boost
#include <boost/noncopyable.hpp>

namespace eows
{
  namespace scidb
  {
    /*!
     * \brief This struct represents a auto complete SciDB query result on destructor.
     *
     * \example
     *   // Retrieving a SciDB connection
     *   eows::scidb::connection conn = eows::scidb::connection_pool::instance().get(array.cluster_id);
     *   // Executing a AFL
     *   boost::shared_ptr<::scidb::QueryResult> qr = conn->execute("subarray(mod13q1, 0, 0, 0, 2, 2, 2)");
     *   // Wrapping Query Result
     *   eows::scidb::scoped_query(qr, &conn);
     *
     * \todo Return a scoped query when execute a query automatically.
     */
    struct scoped_query : private boost::noncopyable
    {
      scoped_query(boost::shared_ptr<::scidb::QueryResult> qr, eows::scidb::connection* c)
        : query_result(qr),
          conn(c)
      {
      }

      ~scoped_query()
      {
        try
        {
          if (query_result != nullptr)
            conn->completed(query_result->queryID);
        }
        catch(...)
        {
          EOWS_LOG_ERROR("Scoped query destructor is throwing exception");
        }
      }

      boost::shared_ptr< ::scidb::QueryResult > query_result; //!< Represents SciDB query result
      eows::scidb::connection* conn; //!< Represents EOWS SciDB connection
    };
  }
}

#endif // __EOWS_SCIDB_SCOPED_QUERY_HPP__
