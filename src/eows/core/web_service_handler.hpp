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
  \file eows/core/web_service_handler.hpp

  \brief A base class for web services.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_CORE_WEB_SERVICE_HANDLER_HPP__
#define __EOWS_CORE_WEB_SERVICE_HANDLER_HPP__

namespace eows
{
  namespace core
  {
    class http_request;
    class http_response;

    //!< A base class for web services.
    class web_service_handler
    {
      public:

        web_service_handler() = default;
      
        virtual ~web_service_handler() = default;
      
        virtual void do_get(const http_request& req, http_response& res);

        virtual void do_head(const http_request& req, http_response& res);

        virtual void do_post(const http_request& req, http_response& res);

        virtual void do_put(const http_request& req, http_response& res);

        virtual void do_delete(const http_request& req, http_response& res);

        virtual void do_options(const http_request& req, http_response& res);

        virtual void do_trace(const http_request& req, http_response& res);

        virtual void do_connect(const http_request& req, http_response& res);
    };

  }   // end namespace core
}     // end namespace eows

#endif // __EOWS_CORE_WEB_SERVICE_HANDLER_HPP__
