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
 *  \brief Web Time Series Classification Service Interface.
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
#include "../geoarray/data_types.hpp"

// C++ Standard Library
#include <memory>

//! Define the EOWS Context.
namespace eows
{
  //! Define the WTSCS Context.
  namespace wtscs
  {

    /*! \brief Check the Status of your Request.
     *
     *  You can use the status operator to display the current status of your request.
     *  You may check this regulary until your request is completed to avoid delays.
     *  This check is based on the following status:
     *  1) Scheduled - The request is submitted.
     *  2) In progress - The request is currently in progress.
     *  3) Completed - The request has been completed.It  will be accompanied by the links to the resulting images.
     *  4) Cancelled - The request has been cancelled.
     *  Example sentence would be: http://localhost:7654/wtscs/status?UUID=123456687
     */
    class status_handler : public eows::core::web_service_handler
    {
      using eows::core::web_service_handler::web_service_handler;

      void do_get(const eows::core::http_request& req, eows::core::http_response& res);
    };

    /*! \brief List the Algorithms Provided by the Service.
     *
     *  Getting a list of available classification algorithms for remote sensing.
     *  Example sentence would be: http://localhost:7654/wtscs/list_algorithms
     */
    class list_algorithms_handler : public eows::core::web_service_handler
    {
      using eows::core::web_service_handler::web_service_handler;


      void do_get(const eows::core::http_request& req, eows::core::http_response& res);
    };


    /*! \brief Describe the Algorithm Provided by the Client.
     *
     *  this is an informal specification to define exactly what the program needs to execute.
     *  Example sentence would be: http://localhost:7654/wtscs/describe_algorithm?algorithm=TWDTW
     */
    class describe_algorithm_handler : public eows::core::web_service_handler
    {
      using eows::core::web_service_handler::web_service_handler;


      void do_get(const eows::core::http_request& req, eows::core::http_response& res);
    };

    /*! \brief Operator Used to Run Process.
     *
     *  This operator processes time series data through a certain algorithm.
     *  You must assign an available processing algorithm and the parameters required.
     *  Example sentence would be: http://localhost:7654/wtscs/run_process
     */

    class run_process_handler : public eows::core::web_service_handler
    {
      using eows::core::web_service_handler::web_service_handler;

      void do_post(const eows::core::http_request& req, eows::core::http_response& res);
    };

    /*! \brief Operator Used to Cancel Process.
     *
     *  This operator is used to cancel processes.
     *  you must inform the UUID of the process.
     *  Example sentence would be: http://localhost:7654/wtscs/cancel_process?UUID=123456687
     */
    class cancel_process_handler : public eows::core::web_service_handler
    {
      using eows::core::web_service_handler::web_service_handler;

      void do_get(const eows::core::http_request& req, eows::core::http_response& res);
    };

    /*! \brief Service Features.
     *
     *  Record the WTSCS service operations.
     */
    void open_request_list();

    void initialize();

  } // end namespace wtscs
} // end namespace eows

#endif // __EOWS_OGC_WMS_WMS_HPP__

