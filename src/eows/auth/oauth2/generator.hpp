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
  \file eows/auth/oauth2/generator.hpp

  \brief Nonce Generator for OAuth2 module.

  \author Raphael Willian da Costa
 */

#ifndef __EOWS_AUTH_OAUTH2_GENERATOR_HPP__
#define __EOWS_AUTH_OAUTH2_GENERATOR_HPP__

// STL
#include <string>
#include <random>

namespace eows
{
  namespace auth
  {
    class nonce_generator
    {
      public:
        nonce_generator(int length = 32);
        ~nonce_generator() = default;

        std::string generate();
        const int& length() const;

        void set_length(const int length);
      private:
        std::mt19937 random_;
        int length_;
    };
  }  // end namespace core
}    // end namespace eows

#endif  // __EOWS_AUTH_OAUTH2_GENERATOR_HPP__
