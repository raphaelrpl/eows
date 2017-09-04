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
  \file eows/auth/routes.hpp

  \brief Defines routes for OAuth2.

  \author Raphael Willian da Costa
 */

#ifndef __EOWS_AUTH_ROUTES_HPP__
#define __EOWS_AUTH_ROUTES_HPP__

// EOWS
#include "../core/web_service_handler.hpp"

namespace eows
{
  namespace auth
  {
    //! Handles OAuth2 Authorize operations
    /*!
      http://localhost:7654/oauth2/authorize
     */
    class oauth2_authorize : public eows::core::web_service_handler
    {
      using eows::core::web_service_handler::web_service_handler;

      /*!
       * \brief Performs Authorization check
       *
       * Tries to check in session for authorization. If found, check role handling.
       *
       * Replies:
       *   - Authorize template : When user is authenticated but role is not allowed yet.
       *   - Login template     : When parameters are ok but no session found;
       *   - Error template     : When an error occurred (forbidden, invalid request)
       *
       * \note This method won't reply JSON content.
       *
       * \param req
       * \param res
       */
      void do_get(const eows::core::http_request& req,
                  eows::core::http_response& res);

      void do_post(const eows::core::http_request& req,
                   eows::core::http_response& res);
    };

    //! Handles OAuth2 Info operations
    /*!
      http://localhost:7654/oauth2/info
     */
    class oauth2_info : public eows::core::web_service_handler
    {
      using eows::core::web_service_handler::web_service_handler;

      void do_get(const eows::core::http_request& req,
                  eows::core::http_response& res);
    };

    //! Handles OAuth2 logout
    /*!
      http://localhost:7654/oauth2/logout
     */
    class oauth2_logout : public eows::core::web_service_handler
    {
      using eows::core::web_service_handler::web_service_handler;

      void do_get(const eows::core::http_request& req,
                  eows::core::http_response& res);
    };
  }
}

#endif // __EOWS_AUTH_ROUTES_HPP__
