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
  \file eows/ogc/wcs/wcs.hpp

  \brief OGC Web Coverage Service.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_OGC_WCS_WCS_HPP__
#define __EOWS_OGC_WCS_WCS_HPP__

// EOWS
#include "../../core/web_service_handler.hpp"

namespace eows
{
  namespace ogc
  {
    namespace wcs
    {
      //! Return a 'Hello from Earth Observation Web Services' or the message sent by the client.
      /*!
        http://localhost:7654/echo

        http://localhost:7654/echo?message="something to test"
       */
      class handler : public eows::core::web_service_handler
      {
        using eows::core::web_service_handler::web_service_handler;
      
        void do_get(const eows::core::http_request& req,
                    eows::core::http_response& res);
      };

      //! Initialize the service.
      void initialize();

    } // end namespace wcs
  }   // end namespace ogc
}     // end namespace eows

#endif  // __EOWS_OGC_WCS_WCS_HPP__

