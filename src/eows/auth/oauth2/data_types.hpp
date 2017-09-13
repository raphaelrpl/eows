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
    /*!
     * \brief Defines an OAuth2 Request Parameters
     *
     * TODO
     */
    struct oauth_parameters
    {
      oauth_parameters() = default;
      oauth_parameters(const eows::core::query_string_t& query_string);

      std::string authorize; //!< Represents intend type. i.e "authorize" for requesting for authorization
      std::string code;      //!< Represents an encrypted code used to exchange for access_token
      std::string username;  //!< Represents username of authenticated user. TODO: Is it required to send in redirect? Security?
      std::string password;  //!< Represents user password used to authenticate user. It must be sent through body content
      std::string access_token;  //!< Represents access token providing temporary direct access to protected resources
      std::string expires_in;    //!< Represents access token expiration
      std::string refresh_token; //!< Represents an token for request new access token once expired
      std::string token_type; //!< Represents kind of token. Default: "Bearer"
      std::string grant_type; //!< Represents grant type method to use
      std::string response_type; //!< Represents requesting type. Default "code"
      std::string client_id; //!< Represents client app id
      std::string client_secret; //!< Represents client secret. see more in TODO...
      std::string redirect_uri;  //!< Represents client redirect URI once everything is matched.
      std::string scope; //!< Represents scope roles to access from protected resources like user information, etc
      std::string state; //!< Represents CSRF Token. TODO: Implement
      std::string error; //!< Represents error type
      std::string error_description; //!< Represents an extra information for error

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

      bool has_redirect_uri(const std::string& uri) const;
      bool has_role(const std::string& role) const;
    };

    struct role_map
    {
      std::map<std::string, std::map<std::string, std::string>> roles;

      bool has_role(const std::string& role_name);
      bool add(const std::string& role);
      void set(const std::string& role, const std::string& key, const std::string& value);
      void remove(const std::string& role);
      void remove(const std::string& role, const std::string& key);
    };

    struct session
    {
      std::string token;
      std::time_t update_time;
      std::string user;
      role_map roles;
//      std::vector<std::string> roles;

      bool has_role(const std::string& role_name);
      bool expired() const;
    };

    /*!
     * \brief OAuth2
     *
     * The OAuth 2.0 spec recommends a maximum lifetime of 10 minutes for authorization code.
     *
     * We will use max lifetime defined in share/eows/config/auth.json
     *
     * \note Because authoriztion codes are meant to be short-live and single-use only,
     *       they are great candidate to implement as self encoded using all necessary
     *       information into the code itself.
     *       See more in https://www.oauth.com/oauth2-servers/authorization/the-authorization-response/
     */
    struct oauth_code
    {
      std::string id;           //!< Unique code to able to check if code has been used before
      std::string client_id;    //!< Client Identifier
      std::string redirect_uri; //!< Redirect URI used. Required to be stored since the access token must contain the same redirect URI
      std::time_t expiration;   //!< Code expiration that only lasts in short time
      std::string user_id;      //!< User identification

      /*!
       * \brief Checks if code entity has been expired.
       *
       * \return OAuth code state
       */
      bool expired() const;
    };
  }
}

#endif // __EOWS_AUTH_OAUTH2_DATA_TYPES_HPP__
