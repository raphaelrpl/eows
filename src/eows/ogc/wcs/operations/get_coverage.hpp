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
  \file eows/ogc/wcs/operations/get_coverage.hpp

  \brief Represents WCS GetCoverage operation

  \author Raphael Willian da Costa
 */
#ifndef __EOWS_OGC_WCS_OPERATIONS_GET_COVERAGE_HPP__
#define __EOWS_OGC_WCS_OPERATIONS_GET_COVERAGE_HPP__

// EOWS
#include "../core/operation.hpp"

// Boost
#include <boost/noncopyable.hpp>

namespace eows
{
  namespace ogc
  {
    namespace wcs
    {
      namespace operations
      {
        // Forward declaration
        struct get_coverage_request;

        class get_coverage : public wcs::core::operation, private boost::noncopyable
        {
          public:
            get_coverage(const get_coverage_request&);

            ~get_coverage();

            /*!
             * \brief It performs a GetCoverage operations, retrieving data from SciDB and modelling from output format.
             * \throws eows::ogc::ogc_error -
             */
            void execute() override;
            const char* content_type() const override;
            const std::string& to_string() const override;
          private:
            struct impl;
            impl* pimpl_;
        };
      }
    }
  }
}

#endif // __EOWS_OGC_WCS_OPERATIONS_GET_COVERAGE_HPP__
