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
    class token_t
    {
      public:
        typedef std::unordered_map<std::string, std::string> metadata_t;

        token_t(const std::string& token);
        token_t(const metadata_t& information);
        ~token_t();

        void digest();
        void attach(const std::string& key, const std::string& value);
        const std::string claim(const std::string& key);
        const metadata_t claim() const;
        const std::string token() const;

        bool expired() const;

        const std::string to_json() const;

      private:
        struct impl;
        impl* pimpl_;
    };
  }  // end namespace core
}    // end namespace eows

#endif  // __EOWS_AUTH_OAUTH2_TOKEN_HPP__
