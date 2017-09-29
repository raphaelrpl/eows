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
  \file eows/auth/data_types.hpp

  \brief Defines datatypes for EOWS Auth module.

  \author Raphael Willian da Costa
 */

#ifndef __EOWS_AUTH_DATA_TYPES_HPP__
#define __EOWS_AUTH_DATA_TYPES_HPP__

// STL
#include <string>

namespace eows
{
  namespace auth
  {
    //! Represents a default Sessin Token Identifier
    static const std::string session_label("session_id");

    /*!
     * \brief Represents OAuth2 module settings
     */
    struct config_t
    {
      std::string oauth2_authorize_uri; //!< Represents path to Authorize URI
      std::string oauth2_logout_uri;    //!< Represents path to Logout URI
      std::string oauth2_info_uri;      //!< Represents path to token information
      std::string oauth2_token_uri;     //!< Represents path to token exchange
      std::string oauth2_login_template_path;   //!< Represents path to OAuth2.0 Login template
      std::string oauth2_message_template_path; //!< Represemts path to OAuth2.0 Authorization template
      std::string oauth2_error_template_path;   //!< Represents path to OAuth2.0 Page error
      std::size_t session_expiration;     //!< Represents User session lifetime cycle.
      std::size_t oauth2_code_expiration; //!< Represents OAuth2 code lifetime cycle.
      bool use_refresh_token; //!< Represents if OAuth2.0 module will support Refresh tokens.
    };

    /*!
     * \brief Represents EOWS User
     */
    struct user_t
    {
      std::string full_name;
      std::string username;
      std::string password;

      bool match(const std::string& user, const std::string& passwd)
      {
        return username == user && password == passwd;
      }
    };
  }
}

#endif // __EOWS_AUTH_DATA_TYPES_HPP__
