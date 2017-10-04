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
  \file eows/core/data_types.hpp

  \brief Some usefull data types for EOWS.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_CORE_DATA_TYPES_HPP__
#define __EOWS_CORE_DATA_TYPES_HPP__

// STL
#include <map>
#include <string>

namespace eows
{
  namespace core
  {

    //! Query string.
    typedef std::multimap<std::string, std::string> query_string_t;

    /*!
      \enum content_type
      
      \brief The content-type (also known as media-type or MIME-type) defines the type format contents transmitted between HTTP server and client.

      \note See https://www.sitepoint.com/web-foundations/mime-types-complete-list/ for a list of well known mime-types.
     */
    enum content_type_t
    {
      APPLICATION_JSON,
      APPLICATION_XML,
      IMAGE_GIF,
      IMAGE_JPEG,
      IMAGE_PNG,
      IMAGE_TIFF,
      IMAGE_X_TIFF,
      TEXT_PLAIN,
      TEXT_XML,
      TEXT_HTML,
      APPLICATION_OCTET_STREAM
    };

    /*!
     * \brief Represents structure for handling HTTP Authorization header.
     *
     * The HTTP Authorization header contains client credentials to authenticate user agent with server.
     * In this way, the authorization defines an security approch to determines access to protected resources
     *
     * \note https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Authorization
     */
    struct authorization_t
    {
      /*!
       * \brief Defines all suported authorization types
       *
       * \note https://developer.mozilla.org/en-US/docs/Web/HTTP/Authentication#Authentication_schemes
       */
      enum class type_t
      {
        basic,  //!< Base64-encoded credentials
        bearer, //!< Bearer tokens - OAuth 2.0
        digest, //!< Nonce Hashed credentials (SHA-256/512, MD5). See RFC-7616.
        hoba,   //!< Http origin bound authentication (digital signature based)
        mutual,  //!< Mutual authentication assures the user that server truly knows the user's encrypted password
        none
      };

      /*!
       * \brief Creates authorization object with pre-defined auth value
       * \param str Authorization string. Example: "Basic someBase64Code"
       */
      authorization_t(const std::string& str)
        : type(type_t::none), value()
      {
        configure(str);
      }

      //! Constructor
      authorization_t()
        : type(type_t::none), value()
      {}

      /*!
       * \brief Performs authorization validation, mapping type and value
       *
       * \todo Should throw exception when type is not mapped?
       *
       * \param str Authorization string
       */
      void configure(const std::string& str)
      {
        const auto pos = str.find(" ");
        if (pos == std::string::npos)
          return; //throw

        const auto wrapper(str.substr(0, pos));

        type_t t;
        if (wrapper == "Basic")
          t = type_t::basic;
        else if (wrapper == "Bearer")
          t = type_t::bearer;
        else if (wrapper == "Digest")
          t = type_t::digest;
        else if (wrapper == "hoba")
          t = type_t::hoba;
        else if (wrapper == "mutual")
          t = type_t::mutual;
        else
          t = type_t::none;

        type = t;
        value = str.substr(pos+1, str.size());
      }

      type_t type;       //!< Represents authorization type
      std::string value; //!< Represents authorization value
    };

  } // end namespace core
}   // end namespace eows

#endif // __EOWS_CORE_DATA_TYPES_HPP__
