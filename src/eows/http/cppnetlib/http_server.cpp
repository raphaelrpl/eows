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
  \file eows/http/cppnetlib/http_server.cpp

  \brief HTTP server based on C++ Network Library.

  \author Gilberto Ribeiro de Queiroz
 */

// EOWS
#include "http_server.hpp"
#include "../../exception.hpp"
#include "../../core/app_settings.hpp"
#include "../../core/defines.hpp"
#include "../../core/logger.hpp"
#include "../../core/service_operations_manager.hpp"
#include "../../core/utils.hpp"
#include "http_request.hpp"
#include "http_response.hpp"

// STL
#include <cstdlib>

// Boost
#include <boost/filesystem.hpp>
//#include <boost/format.hpp>

// C++ Network Library
#include <boost/network/include/http/server.hpp>
#include <boost/network/utils/thread_group.hpp>

namespace eows
{
  namespace http
  {
    namespace cppnetlib
    {
      struct handler_t;

      typedef boost::network::http::server<handler_t> server_t;

      struct server_impl
      {
        server_t server;
        bool running = false;

        server_impl(const server_t::options &options)
          : server(options)
        {
        }

        int run()
        {
          running = true;

          server.run();
          
          return EXIT_SUCCESS;
        }

        void stop()
        {
          running = false;
          
          server.stop();
        }
      };
      
      struct handler_t
      {
        void operator()(server_t::request const& req,
                        const server_t::connection_ptr& conn)
        {
          try
          {
            http_request req_wrapper(req);
            http_response res_wrapper(conn);
            
            eows::core::web_service_handler* h = eows::core::service_operations_manager::instance().get(req_wrapper.path());
            
            eows::core::process(*h, req_wrapper, res_wrapper);
          }
          catch(const std::exception& e)
          {
            conn->set_status(server_t::connection::bad_request);
        
            std::map<std::string, std::string> headers = {
              {"Content-Type", "text/plain"},
            };
        
            conn->set_headers(headers);
        
            conn->write(std::string(e.what()));
        
            //std::cerr << e.what() << std::endl;
          }
          catch(...)
          {
            conn->set_status(server_t::connection::bad_request);
        
            std::map<std::string, std::string> headers = {
              {"Content-Type", "text/plain"},
            };
        
            conn->set_headers(headers);
        
            conn->write(std::string("unknown error!"));
        
            //std::cerr << "unknown error!" << std::endl;
          }
        }
      };
      
      struct cppnetlib_info_t
      {
        std::string listen_address;
        std::string listening_port;
        std::size_t threads;
      };
      
      cppnetlib_info_t load_config()
      {
        const rapidjson::Document& doc = eows::core::app_settings::instance().get();
        
        if(!doc.HasMember("cppnetlib"))
          throw eows::parse_error("Please check key 'cppnetlib' in file '" EOWS_CONFIG_FILE "'.");
  
        const rapidjson::Value& jcppnetlib = doc["cppnetlib"];
      
        if(!jcppnetlib.IsObject())
          throw eows::parse_error("Key 'cppnetlib' must be a valid JSON object in file '" EOWS_CONFIG_FILE "'.");
        
        rapidjson::Value::ConstMemberIterator jlisten_address = jcppnetlib.FindMember("listen_address");

        if((jlisten_address == jcppnetlib.MemberEnd()) || (!jlisten_address->value.IsString()))
          throw eows::parse_error("Please check key 'listen_address' in file '" EOWS_CONFIG_FILE "'.");

        cppnetlib_info_t app_cfg;
        
        app_cfg.listen_address = jlisten_address->value.GetString();
        
        rapidjson::Value::ConstMemberIterator jlistening_port = jcppnetlib.FindMember("listening_port");

        if((jlistening_port == jcppnetlib.MemberEnd()) || (!jlistening_port->value.IsUint()))
          throw eows::parse_error("Please check key 'listening_port' in file '" EOWS_CONFIG_FILE "'.");

        app_cfg.listening_port = std::to_string(jlistening_port->value.GetUint());
        
        rapidjson::Value::ConstMemberIterator jthreads = jcppnetlib.FindMember("threads");

        if((jthreads == jcppnetlib.MemberEnd()) || (!jthreads->value.IsUint()))
          throw eows::parse_error("Please check key 'threads' in file '" EOWS_CONFIG_FILE "'.");

        app_cfg.threads = jthreads->value.GetUint();
        
        return app_cfg;
      }

    } // end namespace cppnetlib
  }   // end namespace http
}     // end namespace eows

int eows::http::cppnetlib::http_server::run()
{
  try
  {
    EOWS_LOG_INFO("Preparing web server...");
    
    cppnetlib_info_t cfg_info = load_config();
    
    handler_t request_handler;
    
    server_t::options options(request_handler);
    
    
    server_impl server(options.thread_pool(std::make_shared<boost::network::utils::thread_pool>(cfg_info.threads))
                              .address(cfg_info.listen_address)
                              .port(cfg_info.listening_port));
    
    return server.run();
  }
  catch (const std::exception& e)
  {
    //std::cerr << "Abnormal termination - exception:" << e.what() << std::endl;
    
    return EXIT_FAILURE;
  }
}

