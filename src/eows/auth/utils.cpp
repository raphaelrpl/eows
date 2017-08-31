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
  \file eows/auth/utils.hpp

  \brief Implementation of Authentication utility helpers

  \author Raphael Willian da Costa
 */

#include "utils.hpp"
#include "manager.hpp"
#include "routes.hpp"
#include "data_types.hpp"

// EOWS Core
#include "../core/logger.hpp"
#include "../core/service_operations_manager.hpp"

void register_routes()
{
  const auto config = eows::auth::manager::instance().settings();

  eows::core::service_operations_manager::instance().insert(
        "/oauth2/" + config.oauth2_authorize_uri,
        std::unique_ptr<eows::auth::oauth2_authorize>(new eows::auth::oauth2_authorize)
        );

  eows::core::service_operations_manager::instance().insert(
        "/oauth2/" + config.oauth2_info_uri,
        std::unique_ptr<eows::auth::oauth2_info>(new eows::auth::oauth2_info)
        );

  eows::core::service_operations_manager::instance().insert(
        "/oauth2/" + config.oauth2_logout_uri,
        std::unique_ptr<eows::auth::oauth2_logout>(new eows::auth::oauth2_logout)
        );
}

void eows::auth::initialize()
{
  EOWS_LOG_INFO("Initializing EOWS Authentication module...");

  manager::instance().initialize();

  // Prepare OAuth2 Routes
  register_routes();

  EOWS_LOG_INFO("EOWS Authentication module loaded.");
}
