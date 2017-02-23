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

/*! \file eows/wtscs/wtscs.hpp
 *
 *  \brief Web Time Series Classification Service.
 *
 *  Web Time Series Classification Service, or WTSCS for short, is a web service for classification of time series data from remote sensing imagery.
 *
 *  \author Eduardo Llapa Rodriguez
 *  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_WTSCS_WTSCS_HPP__
#define __EOWS_WTSCS_WTSCS_HPP__

// EOWS
#include "../core/web_service_handler.hpp"

//! Define the eows context.
namespace eows
{
  //! Define the wtscs context.
  namespace wtscs
  {
 
    /*! \brief list_algorithms_handler Class.
     *
     *  List algorithms managed by the server.
     *  An example request would be: http://localhost:7654/wtscs/list_algorithms
     */
    class list_algorithms_handler : public eows::core::web_service_handler
    {
      using eows::core::web_service_handler::web_service_handler;

      void do_get(const eows::core::http_request& req,
                  eows::core::http_response& res);
    };

    /*! \brief handler Class.
     *
     *  Process a WTSCS request.
     *  An example request would be: http://localhost:7654/wtscs
     */
    class handler : public eows::core::web_service_handler
    {
      using eows::core::web_service_handler::web_service_handler;
      
      void do_get(const eows::core::http_request& req,
                    eows::core::http_response& res);
    };
    
    /*! \brief initialize Function.
     *
     *  Record the wtscs service operations.
     */
    void initialize();

  }   // end namespace wtscs
}     // end namespace eows

#endif  // __EOWS_OGC_WMS_WMS_HPP__
