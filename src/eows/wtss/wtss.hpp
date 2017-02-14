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
  \file eows/wtss/wtss.hpp

  \brief Web Time Series Service.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_WTSS_WTSS_HPP__
#define __EOWS_WTSS_WTSS_HPP__

// EOWS
#include "../core/web_service_handler.hpp"

namespace eows
{
  namespace wtss
  {

    //! List the name of coverages managed by the server.
    /*! http://localhost:7654/wtss/list_coverages */
    class list_coverages_handler : public eows::core::web_service_handler
    {
      using eows::core::web_service_handler::web_service_handler;

      void do_get(const eows::core::http_request& req,
                  eows::core::http_response& res);
    };

    //! List the metadata about a given coverage.
    /*! http://localhost:7654/wtss/describe_coverage?name=mod13q1 */
    class describe_coverage_handler : public eows::core::web_service_handler
    {
      using eows::core::web_service_handler::web_service_handler;

      void do_get(const eows::core::http_request& req,
                  eows::core::http_response& res);
    };

    //! Retrieve the time series for a given coverage.
    /*!
      http://localhost:7654/wtss/time_series?coverage=mod13q1&attributes=red,nir&longitude=-54.0&latitude=-12.0
      http://localhost:7654/wtss/time_series?coverage=mod13q1&attributes=red,nir&longitude=-54.0&latitude=-12.0&start_date=2001-01-01&end_date=2001-01-17
     */
    class time_series_handler : public eows::core::web_service_handler
    {
      using eows::core::web_service_handler::web_service_handler;

      void do_get(const eows::core::http_request& req,
                  eows::core::http_response& res);
    };

    //! Returns the computed locations for a given latitude/longitude.
    /*!
      Use the same parameters as in time_series operation:
      http://localhost:7654/wtss/location?coverage=mod13q1&attributes=red,nir&longitude=-54.0&latitude=-12.0
     */
    class location_handler : public eows::core::web_service_handler
    {
      using eows::core::web_service_handler::web_service_handler;

      void do_get(const eows::core::http_request& req,
                  eows::core::http_response& res);
    };

    //! Initialize the service.
    void initialize();

  }   // end namespace wtss
}     // end namespace eows

#endif  // __EOWS_WTSS_WTSS_HPP__

