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
  \file eows/ogc/wms/utils.hpp

  \brief Utility function for the WMS module.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_OGC_WMS_UTILS_HPP__
#define __EOWS_OGC_WMS_UTILS_HPP__

// EOWS
#include "data_types.hpp"

// RapidJSON
#include <rapidjson/document.h>

namespace eows
{
  namespace ogc
  {
    namespace wms
    {

      void read(const rapidjson::Value& jcapabilities, capabilities_t& capabilities);

      void read(const rapidjson::Value& jservice, service_t& service);

      void read(const rapidjson::Value& jcapability, capability_t& capability);

      void read(const rapidjson::Value& jcontact_information, contact_information_t& contact_information);

      void read(const rapidjson::Value& jrequest, request_t& request);

      void read(const rapidjson::Value& jexception, exception_t& exception);

      void read(const rapidjson::Value& jlayer, layer_t& layer);

    }
  }
}

#endif // __EOWS_OGC_WMS_UTILS_HPP__
