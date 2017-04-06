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
  \file eows/ogc/wcs/operations/get_coverage.cpp

  \brief Implementation of WCS GetCoverage operation

  \author Raphael Willian da Costa
 */

// EOWS
#include "get_coverage.hpp"
#include "data_types.hpp"

// EOWS GeoArray
#include "../../../geoarray/data_types.hpp"
#include "../../../geoarray/geoarray_manager.hpp"


struct eows::ogc::wcs::operations::get_coverage::impl
{
  impl(const eows::ogc::wcs::operations::get_coverage_request& req)
    : request(req), format("application/xml"), output()
  {

  }

  const eows::ogc::wcs::operations::get_coverage_request request;
  std::string format;
  std::string output;
};

eows::ogc::wcs::operations::get_coverage::get_coverage(const eows::ogc::wcs::operations::get_coverage_request& req)
  : pimpl_(new impl(req))
{

}

eows::ogc::wcs::operations::get_coverage::~get_coverage()
{
  delete pimpl_;
}

void eows::ogc::wcs::operations::get_coverage::execute()
{
  try
  {
    // Retrieve GeoArray information
    const geoarray::geoarray_t& array = geoarray::geoarray_manager::instance().get(pimpl_->request.coverage_id);

    // Process client request
    // generateOutput(array);

    pimpl_->output = "Lets pretend it is a image";
  }
  catch(const std::exception& e)
  {
    throw ogc::ogc_error("No such coverage '" + pimpl_->request.coverage_id + "'", "coverageID");
  }
}

const char*eows::ogc::wcs::operations::get_coverage::content_type() const
{
  return pimpl_->format.c_str();
}

const std::string& eows::ogc::wcs::operations::get_coverage::to_string() const
{
  return pimpl_->output;
}
