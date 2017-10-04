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
  \file eows/auth/oauth2/token.hpp

  \brief Defines Token Introspection structure.

  \author Raphael Willian da Costa
 */

#ifndef __EOWS_AUTH_OAUTH2_TOKEN_HPP__
#define __EOWS_AUTH_OAUTH2_TOKEN_HPP__

// STL
#include <string>
#include <ctime>
#include <unordered_map>

namespace eows
{
  namespace auth
  {
    /*!
     * \brief Represents class for Token introspection generation
     *
     * This class implements a JWT token format (RFC-7519).
     *
     * See more in https://tools.ietf.org/html/rfc7519
     *
     * \todo In order to generate or to compare a token, we need a secret key. Should we
     *       use an external key or client app secret?
     */
    class token_t
    {
      public:
        typedef std::unordered_map<std::string, std::string> metadata_t;
        /*!
         * \brief Token Introspection Metadata structure
         *
         * This struct contains all values associated in token generation.
         *
         * \example
         * TODO
         *
         * \todo Use this structure for Token metadata handling
         */
        struct information_t
        {
          std::string scope; //!< Scopes associated with generated token
          std::string client_id;  //!< Client app identifier
          std::string username;   //!< User associated with generated token
          std::string token_type; //!< Type of token used.
          std::string sub; //!< Subject of the token.
          std::string aud; //!< Service specific string identifier
          std::string iss; //!< Issuer of this token
          std::string jti; //!< Identifier for the token
          std::time_t exp; //!< Indicates when token will expire (number in seconds)
          std::time_t iat; //!< Indicates when token was originally issued.
          std::time_t nbf; //!< Indicates when token is not to be used before
          metadata_t metadata; //!< Extra information
        };

        token_t(const std::string& token);
        token_t(const metadata_t& information);
        ~token_t();

        const std::string claim(const std::string& key) const;
        const metadata_t claim() const;
        const std::string token() const;

        /*!
         * \brief Check Token availability.
         *
         * Creates a temporary JWT object to check token expiration.
         *
         * \note It also checks for "active" claim in JWT object.
         *
         * \return Token state
         */
        bool expired() const;

        /*!
         * \brief Retrieves JSON representation of Token. Include all claims
         *
         * \note If token is expired, it will display only key "active".
         *
         * \return JSON String representation
         */
        const std::string to_json() const;

      private:
        struct impl;
        impl* pimpl_;
    };
  }  // end namespace core
}    // end namespace eows

#endif  // __EOWS_AUTH_OAUTH2_TOKEN_HPP__
