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
  \file eows/auth/manager.hpp

  \brief Defines a single manager for Auth configurations.

  \author Raphael Willian da Costa
 */

#ifndef __EOWS_AUTH_MANAGER_HPP__
#define __EOWS_AUTH_MANAGER_HPP__

// STL
#include <string>
#include <vector>
#include <memory>

// Boost
#include <boost/noncopyable.hpp>

namespace eows
{
  namespace core
  {
    class http_request;
    class http_response;
  }
  namespace auth
  {
    struct config_t;
    struct oauth_client;
    struct session;
    struct user_t;
    struct oauth_code;

    class manager : private boost::noncopyable
    {
      public:
        //! Destructor
        virtual ~manager();

        //! Singleton
        static manager& instance();

        //! Retrieves OAuth2 settings
        const config_t& settings() const;

        //! Retrieves OAuth2 Login HTML Template
        const std::string& login_template() const;

        /*!
         * \brief Retrieves cached OAuth2 HTML Template for Authorization
         *
         * You must populate the authorization values using eows::auth::replace.
         *
         * \return Retrieves OAuth2 Authorize template
         */
        const std::string& authorize_template() const;

        /*!
         * \brief Retrieves cached OAuth2 HTML Template for errors
         *
         * You must populate the error values using eows::auth::replace.
         *
         * \return Retrieves OAuth2 error template
         */
        const std::string& error_template() const;

        /*!
         * \brief Initializes Manager instance, loading objects in memory
         *
         * \throws eows::parse_error When could not parse JSON object
         */
        void initialize();

        /*!
         * \brief Search for OAuth2 client.
         *
         * \param client_id - Client Identifier
         *
         * \return Returns a pointer to client or nullptr
         */
        oauth_client* find_client(const std::string& client_id) const;

        /*!
         * \brief Search for Session using Cookie Token
         * \param token
         * \return
         */
        session* find_session(const std::string& token) const;
        session* find_session(const eows::core::http_request& request, eows::core::http_response& response) const;
        user_t* find_user(const std::string& username) const;
        oauth_code* find_code(const std::string& code);

        void create_client(const std::string& type,
                           const std::vector<std::string>& redirect_uris,
                           const std::string& application_name,
                           const std::vector<std::string>& roles,
                           std::string& secret);

        void create_code(std::unique_ptr<oauth_code> code);
        void remove_session(session* s);
      protected:
        manager();

      private:
        struct impl;
        impl* pimpl_;
    };
  }
}

#endif // __EOWS_AUTH_MANAGER_HPP__
