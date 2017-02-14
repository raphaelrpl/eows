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
  \file eows/core/app_settings.hpp

  \brief A singleton that stores the application settings.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_CORE_APP_SETTINGS_HPP__
#define __EOWS_CORE_APP_SETTINGS_HPP__

// STL
#include <string>

// Boost
#include <boost/noncopyable.hpp>

// RapidJSON
#include <rapidjson/document.h>

namespace eows
{
  namespace core
  {
    
    //! A singleton that stores the application settings.
    class app_settings : public boost::noncopyable
    {
      public:
      
        /*!
          \brief Tells EOWS where it is installed.
          
          This folder will be used when searching for auxiliary files, such
          as configuration of servers and loggers.
         
          \exception std::invalid_argument If the directory doesn't have a layout recognized by EOWS.
         */
        void set_base_dir(const std::string& dir);
      
        //! Returns the directory where EOWS is installed.
        const std::string& get_base_dir();
      
        /*!
          \brief Sets the JSON document storing the configuration of EOWS.
         
          \exception std::invalid_argument If overwrite is false and a previously settings is already registered.
         */
        void set(rapidjson::Document doc);
      
        /*!
          \brief Returns the JSON document storing the configuration of EOWS.
         */
        const rapidjson::Document& get() const;

        static app_settings& instance();
      
      private:

        app_settings();
      
        ~app_settings();

// no copy allowed
        app_settings(app_settings&);
        app_settings(app_settings&&);
        app_settings& operator=(app_settings&);
        app_settings& operator=(app_settings&&);
      
      private:
      
        struct impl;
      
        impl* pimpl_;
    };

  }   // end namespace core
}     // end namespace eows

#endif // __EOWS_CORE_APP_SETTINGS_HPP__
