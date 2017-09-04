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
  \file eows/auth/oauth2/data_types.hpp

  \brief Defines for EOWS.

  \author Raphael Willian da Costa
 */

#ifndef __EOWS_AUTH_OAUTH2_DATA_TYPES_HPP__
#define __EOWS_AUTH_OAUTH2_DATA_TYPES_HPP__

#include "../../core/data_types.hpp"

// STL
#include <string>
#include <vector>
#include <ctime>

namespace eows
{
  namespace auth
  {
    struct oauth_parameters
    {
      oauth_parameters() = default;
      oauth_parameters(const eows::core::query_string_t& query_string);

      std::string authorize;
      std::string code;
      std::string username;
      std::string password;
      std::string access_token;
      std::string expires_in;
      std::string refresh_token;
      std::string token_type;
      std::string grant_type;
      std::string response_type;
      std::string client_id;
      std::string client_secret;
      std::string redirect_uri;
      std::string scope;
      std::string state;
      std::string error;
      std::string error_description;

      void clear();
      const std::string to_json() const;
      const eows::core::query_string_t to_query_string() const;

      private:
        void set_property(const eows::core::query_string_t& query_string, const std::string& key, std::string& target);
    };

    struct oauth_client
    {
      std::string id;
      std::string key;
      std::string type;
      std::string application_name;
      std::vector<std::string> roles;
      std::vector<std::string> redirect_uris;
      std::time_t created_at;
    };

    struct session
    {
      std::string token;
      std::time_t update_time;
      std::string user;
      std::vector<std::string> roles;

      bool has_role(const std::string& role_name)
      {
        for(const auto& r: roles)
          if (r == role_name)
            return true;
        return false;
      }

      bool expired() const
      {
        std::time_t now = std::time(nullptr);
        long int seconds = (long int)std::difftime(now, update_time);

        return seconds > 0;
      }
    };
  }
}

#endif // __EOWS_AUTH_OAUTH2_DATA_TYPES_HPP__
