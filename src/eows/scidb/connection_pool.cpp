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
  \file eows/scidb/connection_pool.cpp

  \brief A connection pool for SciDB connections.

  \author Gilberto Ribeiro de Queiroz
 */

// EOWS
#include "connection_pool.hpp"
#include "connection.hpp"
#include "connection_impl.hpp"
#include "data_types.hpp"
#include "exception.hpp"

// STL
#include <algorithm>
#include <cassert>
#include <list>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

// Boost
#include <boost/format.hpp>

struct eows::scidb::connection_pool::impl
{
  // TODO: see how to fine tune mutex locking when acquiring connections!!
  struct pool
  {
    pool(const cluster_info_t& cinfo)
      : cluster_info(cinfo),
        num_connections(0),
        available_connections(0)
    {
    }
    
    ~pool()
    {
      assert(num_connections == available_connections);
      assert(num_connections == connections.size());
      assert(num_connections <= cluster_info.max_connections);
      
// release all connection in the pool
      for(std::size_t i = 0; i != available_connections; ++i)
      {
        delete connections[i];
      }
    }
    
    cluster_info_t cluster_info;
    std::vector<connection_impl*> connections; // note: it works like a stack that never shrink!
    std::size_t num_connections;               // total number of connections in the pool
    std::size_t available_connections;         // number of connections not used, i.e., available for use!
  };
  
  std::list<pool> pool_list;
  std::mutex mtx;
};

eows::scidb::connection
eows::scidb::connection_pool::get(const std::string& cluster_id)
{
  std::lock_guard<std::mutex> lock(pimpl_->mtx);
  
// search cluster connection pool
  std::list<impl::pool>::iterator it =
      std::find_if(pimpl_->pool_list.begin(),
                   pimpl_->pool_list.end(),
                   [&cluster_id](const impl::pool& p) -> bool
                   { return p.cluster_info.id == cluster_id; });

// if a connection pool for the cluster is not found,
// throw an exception to warn this!
  if(it == pimpl_->pool_list.end())
  {
    boost::format err_msg("Could not find a connection pool for cluster '%1%'.");
    throw std::invalid_argument((err_msg % cluster_id).str());
  }

// if connection pool is found but doesn't have
// available connections, let's try to open one more!
  if(it->available_connections == 0)
  {
// if the pool has reached its maximum, throw an exception
    if(it->num_connections == it->cluster_info.max_connections)
    {
      boost::format err_msg("Connection pool for cluster '%1%' reached its limit: %2%.");

      throw connection_pool_limit_error((err_msg % cluster_id % it->num_connections).str());
    }
    
// ok! we can open a new connection!
    std::unique_ptr<connection_impl> conn(new connection_impl(cluster_id));
    
    conn->open(it->cluster_info.coordinator_address, it->cluster_info.coordinator_port);

// just to increase vector size
    it->connections.push_back(conn.get());

// now we have one more physical connection
    ++(it->num_connections);

    assert(it->available_connections <= it->num_connections);
    assert(it->num_connections == it->connections.size());
    assert(it->num_connections <= it->cluster_info.max_connections);

    return connection(conn.release());
  }
  else // ok! there is an available connection in the pool
  {
    --(it->available_connections);

    assert(it->available_connections <= it->num_connections);
    assert(it->num_connections == it->connections.size());
    assert(it->num_connections <= it->cluster_info.max_connections);

    return connection(it->connections[it->available_connections]);
  }
}

void
eows::scidb::connection_pool::add(const cluster_info_t& cluster_info)
{
  std::lock_guard<std::mutex> lock(pimpl_->mtx);
  
// search cluster connection pool
  std::list<impl::pool>::iterator it =
      std::find_if(pimpl_->pool_list.begin(),
                   pimpl_->pool_list.end(),
                   [&cluster_info](const impl::pool& p) -> bool
                   { return p.cluster_info.id == cluster_info.id; });

// if a connection pool already exists for the cluster,
// throw an exception to warn this!
  if(it != pimpl_->pool_list.end())
  {
    boost::format err_msg("There is already a connection pool for cluster '%1%'.");
    throw std::invalid_argument((err_msg % cluster_info.id).str());
  }

// create a new pool
  pimpl_->pool_list.emplace_back(cluster_info);
}

eows::scidb::connection_pool&
eows::scidb::connection_pool::instance()
{
  static connection_pool inst;

  return inst;
}

eows::scidb::connection_pool::connection_pool()
  : pimpl_(new impl)
{
}

eows::scidb::connection_pool::~connection_pool()
{
  delete pimpl_;
}

void
eows::scidb::connection_pool::release(connection_impl* conn)
{
  std::lock_guard<std::mutex> lock(pimpl_->mtx);

  std::list<impl::pool>::iterator it =
      std::find_if(pimpl_->pool_list.begin(),
                   pimpl_->pool_list.end(),
                   [conn](const impl::pool& p) -> bool
                   { return p.cluster_info.id == conn->cluster_id(); });
  
// if a connection pool for the cluster is not found,
// throw an exception to warn this!
  if(it == pimpl_->pool_list.end())
  {
    boost::format err_msg("Could not find a connection pool for cluster '%1%' in order to release a connection.");
    throw std::invalid_argument((err_msg % conn->cluster_id()).str());
  }

// otherwise, return it back!
  it->connections[it->available_connections] = conn;
  ++(it->available_connections);
  
  assert(it->available_connections <= it->num_connections);
  assert(it->num_connections <= it->cluster_info.max_connections);
  assert(it->num_connections == it->connections.size());
}
