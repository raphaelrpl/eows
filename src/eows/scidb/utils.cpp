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
  \file eows/scidb/utils.cpp

  \brief General utility functions.

  \author Gilberto Ribeiro de Queiroz
 */

// EOWS
#include "utils.hpp"
#include "../core/app_settings.hpp"
#include "../core/defines.hpp"
#include "../core/logger.hpp"
#include "connection_pool.hpp"
#include "data_types.hpp"
#include "exception.hpp"

// STL
#include <vector>

// Boost
#include <boost/filesystem.hpp>
#include <boost/format.hpp>


eows::scidb::cluster_info_t read(const rapidjson::Value& jcluster)
{
  if(!jcluster.IsObject())
    throw eows::parse_error("Please check cluster elements in file '" EOWS_CONFIG_FILE "'. They must be valid JSON objects.");

  eows::scidb::cluster_info_t cluster;
  
  rapidjson::Value::ConstMemberIterator jid = jcluster.FindMember("id");

  if((jid == jcluster.MemberEnd()) || (!jid->value.IsString()))
    throw eows::parse_error("Please check key 'id' in file '" EOWS_CONFIG_FILE "'.");

  cluster.id = jid->value.GetString();
        
  rapidjson::Value::ConstMemberIterator jcoordinator_address = jcluster.FindMember("coordinator_address");

  if((jcoordinator_address == jcluster.MemberEnd()) || (!jcoordinator_address->value.IsString()))
    boost::format err_msg("Please check key 'coordinator_address' in file '" EOWS_CONFIG_FILE "'.");

  cluster.coordinator_address = jcoordinator_address->value.GetString();
  
  rapidjson::Value::ConstMemberIterator jcoordinator_port = jcluster.FindMember("coordinator_port");

  if((jcoordinator_port == jcluster.MemberEnd()) || (!jcoordinator_port->value.IsUint()))
    throw eows::parse_error("Please check key 'coordinator_port' in file '" EOWS_CONFIG_FILE "'.");

  cluster.coordinator_port = static_cast<uint16_t>(jcoordinator_port->value.GetUint());
        
  rapidjson::Value::ConstMemberIterator jmax_connections = jcluster.FindMember("max_connections");

  if((jmax_connections == jcluster.MemberEnd()) || (!jmax_connections->value.IsUint()))
    throw eows::parse_error("Please check key 'max_connections' in file '" EOWS_CONFIG_FILE "'.");

  cluster.max_connections = jmax_connections->value.GetUint();
        
  return cluster;
}


static std::vector<eows::scidb::cluster_info_t> load_config()
{
  const rapidjson::Document& doc = eows::core::app_settings::instance().get();
  
  if(!doc.HasMember("scidb_clusters"))
    throw eows::parse_error("Please check key 'scidb_clusters' in file '" EOWS_CONFIG_FILE "'.");
  
  const rapidjson::Value& jclusters = doc["scidb_clusters"];
  
  if(!jclusters.IsArray())
    throw eows::parse_error("Key 'scidb_clusters' in file '" EOWS_CONFIG_FILE "' must be a valid JSON array of objects.");
  
  std::vector<eows::scidb::cluster_info_t> clusters;
  
  for(rapidjson::SizeType i = 0; i < jclusters.Size(); ++i)
  {
    eows::scidb::cluster_info_t cinfo = read(jclusters[i]);
    
    clusters.push_back(cinfo);
  }
        
  return clusters;
}

void eows::scidb::initialize()
{
  EOWS_LOG_INFO("Initializing SciDB runtime module...");
  
  EOWS_LOG_INFO("Loading information about SciDB clusters...");
  
  std::vector<eows::scidb::cluster_info_t> clusters = load_config();
  
  EOWS_LOG_INFO("Cluster information loaded!");
  
  for(const auto& cluster : clusters)
  {
    boost::format log_msg("Registering cluster '%1%' in the connection pool.");
    
    EOWS_LOG_INFO((log_msg % cluster.id).str());
    
    connection_pool::instance().add(cluster);
  }
  
  EOWS_LOG_INFO("SciDB runtime module initialized!");
}

