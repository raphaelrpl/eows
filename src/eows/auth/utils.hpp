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
  \file eows/auth/utils.hpp

  \brief Defines utility helpers for auth module

  \author Raphael Willian da Costa
 */

#ifndef __EOWS_AUTH_UTILS_HPP__
#define __EOWS_AUTH_UTILS_HPP__

// STL
#include <string>
#include <map>

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
    /*!
     * \brief Performs Authentication module initialization
     *
     * \throws std::runtime_error When could not intialize module
     */
    void initialize();

    /*!
     * \brief Replaces special tags from template using map information.
     *
     * \param tpl - String  template to replace
     * \param values - Map values to replace for.
     * \param prefix - Tag prefix to match
     * \param postfix - Tag postfix to match
     */
    void replace(std::string& tpl, const std::multimap<std::string, std::string>& values, const std::string& prefix = "{{", const std::string& postfix = "}}");

    /*!
     * \brief Checks if provided access_token is valids and contains specified role.
     *
     * This methods retrieves an access_token from Authorization Header and then applies token validation.
     *
     * \note Injects 401 (Unauthorized) when token is invalid and 200 (OK) for successfully validation.
     *
     * \note Injects WWW-Authenticate with required scope When token is invalid or insufficient permissions.
     *
     * \todo Should throw specific exception such invalid_request_error|token_error etc
     *       for handling permission state?
     *
     * \param role_name - Role name to compare
     * \param request - HTTP Request to retrieve Authorization Header
     * \param response - HTTP Response
     * \return True for valid access_token. Otherwise, False
     */
    bool has_permission_to(const std::string& role_name,
                           const eows::core::http_request& request,
                           eows::core::http_response& response);

    std::string encrypt(const std::string& text, std::string password);
    std::string decrypt(const std::string& text, std::string password);

    std::string generate(const int& length);
  }
}

#endif // __EOWS_AUTH_UTILS_HPP__
