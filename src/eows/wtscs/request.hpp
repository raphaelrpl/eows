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

#ifndef REQUEST_HPP
#define REQUEST_HPP

// STL
#include <string>
#include <vector>
#include <memory>

using namespace std;

//! Define the EOWS Context.
namespace eows
{
  //! Define the WTSCS Context.
  namespace wtscs
  {

    /*! \brief Generic input parameters structure.
         *
         *
         */
    struct base_input_parameters
    {
      virtual ~base_input_parameters() = default;
    };

    /*! \brief Input parameters structure for TWDTW algorithm.
         *
         *
         */
    struct twdtw_input_parameters : base_input_parameters
    {
      string cv_name;
      vector<string> attributes;
      vector<double> roi;
      string start_date;
      string end_date;
      string by;
      double overlap;
      double alpha;
      double beta;
    };

    /*! \brief Binomial structure algorithm-input parameters.
         *
         *
         */
    class request
    {
      string algorithm;
      unique_ptr<base_input_parameters> input_parameters;
      string UUID;
      string status;
    public:
      request* pNext;
      request();
      /*! \brief Parse Request.
       *
       *  The request is parsed into Document Object Model (DOM).
       */
      void set_parameters(const char *, string);
      void set_UUID(string);
      string get_status();
      void check_parameters();
      string unique_identifier();
    };

  } // end namespace wtscs
} // end namespace eows

#endif // request_HPP
