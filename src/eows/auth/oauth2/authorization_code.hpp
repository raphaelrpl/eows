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
  \file eows/auth/defines.hpp

  \brief Defines for EOWS.

  \author Raphael Willian da Costa
 */

#ifndef __EOWS_AUTH_OAUTH2_AUTHORIZATION_CODE_HPP__
#define __EOWS_AUTH_OAUTH2_AUTHORIZATION_CODE_HPP__

// EOWS
#include "data_types.hpp"

namespace eows
{
  namespace core
  {
    class http_request;
    class http_response;
  }
  namespace auth
  {
    struct oauth_parameters;

    class authorization_code
    {
      public:
        /*!
         * \brief Constructor
         *
         * Build Authorization Code Grant handling from oauth_parameters sent in request
         *
         * \param p Client Request OAuth2 parameters
         */
        authorization_code(const oauth_parameters& p);

        /*!
         * \brief Try to apply grant execution into web request
         *
         * This methods will try validate user request, seeking for required credentials
         *
         * \exception ...TODO
         *
         * \param request Client HTTP Request object
         * \param response Client HTTP Response object
         * \return Validated oauth_parameters request
         */
        eows::auth::oauth_parameters grant(const core::http_request& request, core::http_response& response);

        /*!
         * \brief Retrieves client data information once everything is validated
         * \return JSON information
         */
        const std::string information();

        /*!
         * \brief Performs OAuth2 client validations
         *
         * This methods checks client_id identity, looking into cache elements and then credentials validations
         *
         * \param client_id Client OAuth2 requested
         * \param oresp Target output oauth parameters based in valid client
         */
        void validate_client(const std::string &client_id, oauth_parameters &oresp);

        /*!
         * \brief Performs OAuth2 scope validation and set the result to output parameters
         * \param oresp
         */
        void validate_scope(oauth_parameters& oresp);

      protected:
        oauth_parameters params_;
    };
  }
}

#endif // __EOWS_AUTH_OAUTH2_AUTHORIZATION_CODE_HPP__