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
  \file eows/auth/exception.hpp

  \brief Specific exception types for Auth Runtime module.

  \author Raphael Willian da Costa
 */

#ifndef __EOWS_AUTH_OAUTH2_EXCEPTION_HPP__
#define __EOWS_AUTH_OAUTH2_EXCEPTION_HPP__

// EOWS
#include "../exception.hpp"

namespace eows
{
  //! The namespace for the Auth Runtime module of EOWS.
  namespace auth
  {
    struct oauth2_error : public eows_error
    {
      std::string error;
      std::string error_description;

      oauth2_error(const std::string& err,
                   const std::string& err_description)
        : eows_error(err_description),
          error(err),
          error_description(err_description)
      {
      }
    };

    /*!
     * \brief Unauthorized Error
     *
     * This exception is thrown when client is not allowed to
     * request an authorization code using this method.
     */
    struct unauthorized_error : public virtual oauth2_error
    {
      unauthorized_error(const std::string& err_description)
        : oauth2_error("unauthorized_error", err_description)
      {
      }
    };

    /*!
     * \brief Invalid Request Error
     *
     * This exception is thrown when request is missing a parameter
     * or contains an invalid parameters, includes a parameters
     * more than once, etc.
     */
    struct invalid_request_error : public virtual oauth2_error
    {
      invalid_request_error(const std::string& err_description)
        : oauth2_error("invalid_request", err_description)
      {
      }
    };

    /*!
     * \brief The access_denied_error struct
     */
    struct access_denied_error : public virtual oauth2_error
    {
      access_denied_error(const std::string& err_description)
        : oauth2_error("access_denied", err_description)
      {
      }
    };

    struct invalid_scope_error : public virtual oauth2_error
    {
      invalid_scope_error(const std::string& err_description)
        : oauth2_error("invalid_scope", err_description)
      {
      }
    };

    struct unsupported_response_type_error : public virtual oauth2_error
    {
      unsupported_response_type_error(const std::string& err_description)
        : oauth2_error("unsupported_response_type", err_description)
      {
      }
    };

    struct server_error : public virtual oauth2_error
    {
      server_error(const std::string& err_description)
        : oauth2_error("server_error", err_description)
      {
      }
    };

    struct temporarily_unavailable_error : public virtual oauth2_error
    {
      temporarily_unavailable_error(const std::string& err_description)
        : oauth2_error("temporarily_unavailable", err_description)
      {
      }
    };
  }  // end namespace core
}    // end namespace eows

#endif  // __EOWS_AUTH_OAUTH2_EXCEPTION_HPP__
