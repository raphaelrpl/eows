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
  \file eows/scidb/connection_impl.cpp

  \brief The type of connection managed by the connection pool.

  \author Gilberto Ribeiro de Queiroz
 */

// EOWS
#include "connection_impl.hpp"
#include "exception.hpp"

// Boost
#include <boost/format.hpp>

// SciDB
#include <system/Constants.h>

eows::scidb::connection_impl::~connection_impl()
{
  try
  {
    close();
  }
  catch(...)
  {
  }
}

void
eows::scidb::connection_impl::open(const std::string& coordinator_address,
                                   const uint16_t coordinator_port)
{
  if(coordinator_address.empty())
    throw std::invalid_argument("Coordinator server address can not be empty.");
  
  if(coordinator_port == 0)
    throw std::invalid_argument("Coordinator server port number can not be 0.");

  close();
  
  try
  {
#if EOWS_SCIDB_MAJOR_VERSION < 16
    const ::scidb::SciDB& db_api = ::scidb::getSciDB();

    handle_ = db_api.connect(coordinator_address, coordinator_port);
#else
    ::scidb::SciDB& db_api = ::scidb::getSciDB();

    ::scidb::SessionProperties sp;

    handle_ = db_api.connect(sp, coordinator_address, coordinator_port);
#endif

  }
  catch(const ::scidb::Exception& e)
  {
    throw connection_open_error(e.what());
  }
  catch(...)
  {
    throw connection_open_error("unknown error trying to connect to the coordinator server.");
  }
  
  if(handle_ == nullptr)
  {
    throw connection_open_error("unknown error trying to connect to the coordinator server: a null handle was not expected.");
  }
}

void
eows::scidb::connection_impl::close()
{
  if(handle_)
  {
    
#if EOWS_SCIDB_MAJOR_VERSION < 16
    const ::scidb::SciDB& db_api = ::scidb::getSciDB();
#else
    ::scidb::SciDB& db_api = ::scidb::getSciDB();
#endif
    
    try
    {
      db_api.disconnect(handle_);
    }
    catch(const ::scidb::Exception& e)
    {
      throw connection_close_error(e.what());
    }
    catch(...)
    {
      throw connection_close_error("unknown error trying to close connection with coordinator server.");
    }
    
    handle_ = nullptr;
  }
}

bool
eows::scidb::connection_impl::is_open() const
{
  return handle_ != nullptr;
}

bool
eows::scidb::connection_impl::is_closed() const
{
  return handle_ == nullptr;
}

boost::shared_ptr<scidb::QueryResult>
eows::scidb::connection_impl::execute(const std::string& query_str, const bool afl)
{
  boost::shared_ptr< ::scidb::QueryResult > qresult(new ::scidb::QueryResult);

#if EOWS_SCIDB_MAJOR_VERSION < 16
    const ::scidb::SciDB& db_api = ::scidb::getSciDB();
#else
    ::scidb::SciDB& db_api = ::scidb::getSciDB();
#endif

  try
  {
    db_api.executeQuery(query_str, afl, *qresult, handle_);
  }
  catch(const ::scidb::Exception& e)
  {
    throw query_execution_error(e.what());
  }
  catch(...)
  {
    boost::format err_msg("unknown error executing the query: '%1%'.");
   
    throw query_execution_error((err_msg % query_str).str());
  }

  return qresult;
}

void
eows::scidb::connection_impl::completed(::scidb::QueryID id)
{
#if EOWS_SCIDB_MAJOR_VERSION < 16
    const ::scidb::SciDB& db_api = ::scidb::getSciDB();
#else
    ::scidb::SciDB& db_api = ::scidb::getSciDB();
#endif

  db_api.completeQuery(id, handle_);
}

eows::scidb::connection_impl::connection_impl(std::string cluster_id)
  : handle_(nullptr), cluster_id_(cluster_id)
{
}

