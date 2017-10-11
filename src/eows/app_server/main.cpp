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
  \file eows/app_server/main.cpp

  \brief An HTTP Web Server for Earth Observation Web Services.

  \author Gilberto Ribeiro de Queiroz
 */

// EOWS
#include "../exception.hpp"
#include "../version.hpp"
#include "../core/app_settings.hpp"
#include "../core/defines.hpp"
#include "../core/http_server.hpp"
#include "../core/http_server_builder.hpp"
#include "../core/logger.hpp"
#include "../core/utils.hpp"

#ifdef EOWS_HTTP_CPPNETLIB_ENABLED
#include "../http/cppnetlib/http_server.hpp"
#include "../http/cppnetlib/utils.hpp"
#endif

#ifdef EOWS_HTTP_CROW_ENABLED
#include "../http/crow/http_server.hpp"
#include "../http/crow/utils.hpp"
#endif

#ifdef EOWS_SCIDB_ENABLED
#include "../scidb/utils.hpp"
#endif

#ifdef EOWS_PROJ4_ENABLED
#include "../proj4/utils.hpp"
#endif

#ifdef EOWS_GDAL_ENABLED
#include "../gdal/utils.hpp"
#endif

#ifdef EOWS_GEOARRAY_ENABLED
#include "../geoarray/utils.hpp"
#endif

#ifdef EOWS_GEOCACHE_ENABLED
#include "../geocache/utils.hpp"
#endif

#ifdef EOWS_OGC_OWS_ENABLED
#include "../ogc/ows/ows.hpp"
#endif

#ifdef EOWS_SERVICE_ECHO_ENABLED
#include "../echo/echo.hpp"
#endif

#ifdef EOWS_SERVICE_WTSS_ENABLED
#include "../wtss/wtss.hpp"
#endif

#ifdef EOWS_SERVICE_WCS_ENABLED
#include "../ogc/wcs/wcs.hpp"
#endif

#ifdef EOWS_SERVICE_WMS_ENABLED
#include "../ogc/wms/wms.hpp"
#endif

#ifdef EOWS_SERVICE_WTSCS_ENABLED
#include "../wtscs/wtscs.hpp"
#endif

// STL
#include <cstdlib>
#include <iostream>
#include <string>

// Boost
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/program_options.hpp>

int main(int argc, char *argv[])
{
  try
  {
// retrieve command line options
    std::string base_dir;
    
    boost::program_options::options_description options_all("Earth Observation Web Services Options");
    
    options_all.add_options()
    ("version", "Print EOWS version.\n")
    ("help", "Prints help message.\n")
    ("base-dir", boost::program_options::value<std::string>(&base_dir), "The base directory where EOWS is installed.\n")
    ;
    
    boost::program_options::variables_map options;
    
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, options_all), options);
    
    if(options.count("help") || options.empty())
    {
      std::cout << options_all << std::endl;
      
      return EXIT_SUCCESS;
    }
    
    boost::program_options::notify(options);

    if(options.count("version"))
    {
      std::cout << "\n\nEarth Observation Web Services version " EOWS_VERSION_STRING "\n" << std::endl;
      
      return EXIT_SUCCESS;
    }

    if(base_dir.empty())
    {
      throw std::invalid_argument("Option '--base-dir' can not be empty.");
    }

// set application dir and load its JSON configuration file
    eows::core::app_settings::instance().set_base_dir(base_dir);
    
    boost::filesystem::path cfg_file(base_dir);
  
    cfg_file /= EOWS_CONFIG_FILE;

    std::string scfg = cfg_file.string();

    eows::core::app_settings::instance().set(eows::core::open_json_file(scfg));
  
// intialize the system

    eows::core::initialize();
  }
  catch(const std::exception& e)
  {
    boost::format err_msg("EOWS error: %1%.");
    
    std::cerr << (err_msg % e.what()).str() << std::endl;
    
    return EXIT_FAILURE;
  }
  catch(...)
  {
    std::cerr << "An unknown error occurred!" << std::endl;

    return EXIT_FAILURE;
  }

// from here on we can use the application LOGGER
  try
  {

#ifdef EOWS_PROJ4_ENABLED
    eows::proj4::initialize();
#endif

#ifdef EOWS_GDAL_ENABLED
    eows::gdal::initialize();
#endif

#ifdef EOWS_SCIDB_ENABLED
    eows::scidb::initialize();
#endif

#ifdef EOWS_GEOARRAY_ENABLED
    eows::geoarray::initialize();
#endif

#ifdef EOWS_GEOCACHE_ENABLED
    eows::geocache::initialize();
#endif

#ifdef EOWS_OGC_OWS_ENABLED
    eows::ogc::ows::initialize();
#endif
    
#ifdef EOWS_SERVICE_ECHO_ENABLED
    eows::echo::initialize(); 
#endif

#ifdef EOWS_SERVICE_WTSS_ENABLED
    eows::wtss::initialize();
#endif

#ifdef EOWS_SERVICE_WCS_ENABLED
    eows::ogc::wcs::initialize();
#endif

#ifdef EOWS_SERVICE_WMS_ENABLED
    eows::ogc::wms::initialize();
#endif

#ifdef EOWS_SERVICE_WTSCS_ENABLED
    eows::wtscs::initialize();
#endif

#ifdef EOWS_HTTP_CPPNETLIB_ENABLED
    eows::http::cppnetlib::initialize();
#endif

#ifdef EOWS_HTTP_CROW_ENABLED
    eows::http::crow::initialize();
#endif

// find out the HTTP server to be used
    const rapidjson::Document& jdoc = eows::core::app_settings::instance().get();
    
    rapidjson::Value::ConstMemberIterator jhttp_server = jdoc.FindMember("http_server");

    if((jhttp_server == jdoc.MemberEnd()) || (!jhttp_server->value.IsString()))
      throw eows::parse_error("Please check key 'http_server' in file '" EOWS_CONFIG_FILE "'.");

    const std::string http_server_name = jhttp_server->value.GetString();

    std::unique_ptr<eows::core::http_server> server(eows::core::http_server_builder::instance().build(http_server_name));

    return server->run();
  }
  catch(const std::exception& e)
  {
    boost::format err_msg("The following error has occurred: %1%.");

    std::cerr << (err_msg % e.what()).str() << std::endl;

    EOWS_LOG_FATAL((err_msg % e.what()).str());

    return EXIT_FAILURE;
  }
  catch(...)
  {
    std::cerr << "An unknown error occurred!" << std::endl;
    
    EOWS_LOG_FATAL("An unknown error occurred!");

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

