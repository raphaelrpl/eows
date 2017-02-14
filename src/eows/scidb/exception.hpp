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
  \file eows/scidb/exception.hpp

  \brief Specific exception types for SciDB module.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_SCIDB_EXCEPTION_HPP__
#define __EOWS_SCIDB_EXCEPTION_HPP__

// EOWS
#include "../exception.hpp"

namespace eows
{
  //! The namespace for the SciDB module of EOWS.
  namespace scidb
  {

    //! An exception indicating an error when opening a connection to a database cluster.
    struct connection_open_error: virtual std::runtime_error
    {
      explicit connection_open_error(const std::string& s) : std::runtime_error(s) {}
    
      explicit connection_open_error(const char* s) : std::runtime_error(s) {}
    };
    
    //! An exception indicating an error when closing the connection to a database cluster.
    struct connection_close_error: virtual std::runtime_error
    {
      explicit connection_close_error(const std::string& s) : std::runtime_error(s) {}
    
      explicit connection_close_error(const char* s) : std::runtime_error(s) {}
    };
    
    //! An exception indicating an error when executing a query.
    struct query_execution_error: virtual std::runtime_error
    {
      explicit query_execution_error(const std::string& s) : std::runtime_error(s) {}
    
      explicit query_execution_error(const char* s) : std::runtime_error(s) {}
    };
    
    //! An exception indicating that connection pool has reached its limit.
    struct connection_pool_limit_error: virtual std::runtime_error
    {
      explicit connection_pool_limit_error(const std::string& s) : std::runtime_error(s) {}
    
      explicit connection_pool_limit_error(const char* s) : std::runtime_error(s) {}
    };
    

  }  // end namespace scidb
}    // end namespace eows

#endif  // __EOWS_SCIDB_EXCEPTION_HPP__
