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
     * \brief Defines common parameters used in OAuth 2.0 Request.
     */
    struct oauth_parameters
    {
      //! Constructor
      oauth_parameters() = default;

      //! Constructor. Initializes oauth parameters from query string
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

      //! Initializes OAuth2 parameters, filling out from query_string
      void configure(const eows::core::query_string_t& query_string);

      //! Performs OAuth2 Parameters cleanup
      void clear();

      /*!
       * \brief Retrieves a JSON string representation of OAuth2 Parameters
       *
       * \return JSON string
       */
      const std::string to_json() const;

      /*!
       * \brief Transforms oauth parameters into Query String object
       *
       * \return Query string values
       */
      const eows::core::query_string_t to_query_string() const;

      private:
        //! Helper to configure each oauth parameter
        void set_property(const eows::core::query_string_t& query_string, const std::string& key, std::string& target);
    };

    /*!
     * \brief Represents Application Thirdparties application that will use EOWS OAuth2 funcionalities
     *
     * An OAuth2 Client is classified in two distinct types:
     *
     * - public
     *
     *   Represents clients that are cannot maintain the confidentiality of a *client_secret*. In other words, the secret is not
     *   used for these apps (Mobile Apps and Javascripts apps).
     *
     * - confidential
     *
     *   Represents clients which have hability to maintain the confidentiality of the *client_secret*. Typically, they are
     *   applications that the source code is not accessible to users. (web-apps)
     */
    struct oauth_client
    {
      std::string id;                         //!< Client Identifier used to generate authorize URI
      std::string key;                        //!< Client Key used to generate a secret
      std::string type;                       //!< Type of client. Use "public" or "confidential". TODO: Document and implement each one. Default "public"
      std::string application_name;           //!< App name to show when requesting user permission
      std::vector<std::string> roles;         //!< All supported roles that App will provide.
      std::vector<std::string> redirect_uris; //!< All supported URIs that OAuth2 should trust to redirect
      std::time_t created_at;                 //!< Time creation of app

      /*!
       * \brief Checks whether uri provides exists in pre-defined app URIs
       *
       * \param uri - URI to check
       * \return Bool
       */
      bool has_redirect_uri(const std::string& uri) const;

      /*!
       * \brief Checks whether role provided exists in pre-defined app roles
       *
       * \param role - Role to check
       * \return Bool
       */
      bool has_role(const std::string& role) const;
    };

    /*!
     * \brief Represents Role Map for User Session useful for handle user permissions and user preferences.
     *
     * A role map contains following structure:
     *
     * {
     *   "role_key": {
     *     "key_name": "value"
     *     ...
     *   }
     *   ...
     * }
     *
     * In OAuth2 concept, every role must contain an access_token associated to be able to validate when user requests.
     */
    struct role_map
    {
      std::map<std::string, std::map<std::string, std::string>> roles;

      /*!
       * \brief Checks if a given role exists
       *
       * \param role_name - Role name to check
       * \return Bool state
       */
      bool has_role(const std::string& role_name);

      /*!
       * \brief Check if inside role, exists provided key
       *
       * \param role_name - Role name to check
       * \param key - Property key
       * \return Bool state
       */
      bool has_role(const std::string &role_name, const std::string& key);

      /*!
       * \brief Creates a new role map entry
       *
       * \param role - Role name
       * \return True when role has created or False when role already exists
       */
      bool add(const std::string& role);

      /*!
       * \brief Inserts role key and store element inside map
       *
       * \param role - Role name
       * \param key - Sub role key
       * \param value - Value to store
       */
      void set(const std::string& role, const std::string& key, const std::string& value);

      //! Removes a role from map
      void remove(const std::string& role);

      //! Removes a role key from map
      void remove(const std::string& role, const std::string& key);
    };

    /*!
     * \brief Represents User session with EOWS server
     *
     * Sessions are unique, they are stored and retrieved with a token passed through a cookie, a JSON or query string.
     * The sessions have roles used to manage user permissions and store useful properties like user preferences,
     * locales etc.
     *
     * \todo Define a token pattern generation for cookie session. Currently, its generating a MD5 with 32 bits
     *
     * \note All session values is stored in server side
     *
     * \example
     *
     * // Find User session or create a new one. Once retrieves, it already add "Set-Cookie" in response object
     * auto s = manager::instance().find_session(request, response);
     *
     * s->roles.add("mycachedata");
     */
    struct session
    {
      std::string token;       //!< Session or cookie token to pass through HTTP Header
      std::time_t update_time; //!< Last update time when session is accessed
      std::string user;        //!< Identifier to detect whether user is logged or not.
      role_map roles;          //!< Roles associated with User Session. This session is created with cookie to user send automatically entity

      /*!
       * \brief Checks whether session is expired or not
       *
       * \return Session validity state
       */
      bool expired() const;
    };

    /*!
     * \brief OAuth2 Code for access code exchange process
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
      std::vector<std::string> roles; //!< Roles associated with authorization code

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
