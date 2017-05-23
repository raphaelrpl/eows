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
      TEXT_HTML
    };
    
  } // end namespace core
}   // end namespace eows

#endif // __EOWS_CORE_DATA_TYPES_HPP__
