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

// Boost
#include <boost/noncopyable.hpp>

namespace eows
{
  namespace core
  {
    class http_request;
  }
  namespace auth
  {
    struct config_t;
    struct oauth_client;
    struct session;
    struct user_t;

    class manager : private boost::noncopyable
    {
      public:
        virtual ~manager();
        static manager& instance();

        const config_t& settings() const;

        const std::string& login_template() const;
        const std::string& authorize_template() const;
        const std::string& error_template() const;
        void initialize();

        oauth_client* find_client(const std::string& client_id) const;
        session* find_session(const std::string& token) const;
        session* find_session(const eows::core::http_request& request) const;
        user_t* find_user(const std::string& username) const;

        bool authenticate(const std::string& username, const std::string& password);

        void create_client(const std::string& type,
                           const std::vector<std::string>& redirect_uris,
                           const std::string& application_name,
                           const std::vector<std::string>& roles,
                           std::string& secret);

        void create_session(const user_t& user);
      protected:
        manager();

      private:
        struct impl;
        impl* pimpl_;
    };
  }
}

#endif // __EOWS_AUTH_MANAGER_HPP__
