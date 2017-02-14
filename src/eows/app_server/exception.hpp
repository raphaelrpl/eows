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
  \file eows/app_server/exception.hpp

  \brief Specific exception types in the Appplication Server module.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_APP_SERVER_EXCEPTION_HPP__
#define __EOWS_APP_SERVER_EXCEPTION_HPP__

// EOWS
#include "../exception.hpp"

namespace eows
{
  //! The namespace for the Application Server module of EOWS.
  namespace server
  {

    //! The base type for exceptions from the core runtime module.
    //struct exception: virtual eows::exception { };

    //! An exception indicating that a service operation could not be registered or it could not be found in the list of valid operations.
    //struct invalid_service_operation_error: virtual exception { };

  }  // end namespace server
}    // end namespace eows

#endif  // __EOWS_APP_SERVER_EXCEPTION_HPP__
