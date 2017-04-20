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
  \file eows/ogc/wms/wms.cpp

  \brief OGC Web Map Service.

  \author Gilberto Ribeiro de Queiroz
 */

// EOWS
#include "wms.hpp"
#include "../../core/http_response.hpp"
#include "../../core/http_request.hpp"
#include "../../core/logger.hpp"
#include "../../core/service_operations_manager.hpp"
#include "data_types.hpp"
#include "manager.hpp"

// STL
#include <iterator>

// Boost
#include <boost/format.hpp>

// RapidXML
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>

using namespace rapidxml;

namespace eows
{
  namespace ogc
  {
    namespace wms
    {
      static void
      return_exception(const char* msg, eows::core::http_response& res);

      struct GetCapabilitiesRequest
      {
        std::string version;
        std::string service;
        std::string format;
        std::string updatesequence;
      };
    }
  }
}

void eows::ogc::wms::handler::do_get(const eows::core::http_request& req,
                                     eows::core::http_response& res)
{

  try
  {
    eows::core::query_string_t qstr(req.query_string());

    eows::core::query_string_t::const_iterator it = qstr.find("REQUEST");

    if(it == qstr.end())
      throw std::runtime_error("Missing \"REQUEST\" parameter in WMS query.");

    if(it->second == "GetCapabilities")
    {

    }
    else if(it->second == "GetMap")
    {

    }
    else if(it->second == "GetFeatureInfo")
    {

    }
    else
    {
      boost::format err_msg("Wrong parameter in WMS \"REQUEST\": %1%.");

      throw std::runtime_error((err_msg % it->second).str());
    }

    //res.set_status(eows::core::http_response::OK);

    //res.add_header(eows::core::http_response::CONTENT_TYPE, "text/plain; charset=utf-8");
    //res.add_header(eows::core::http_response::ACCESS_CONTROL_ALLOW_ORIGIN, "*");

    //res.write(return_msg.c_str(), return_msg.size());
  }
  catch(const std::exception& e)
  {
    return_exception(e.what(), res);
  }
  catch(...)
  {
    return_exception("Unexpected error in WMS.", res);
  }
}

void
eows::ogc::wms::initialize()
{
  EOWS_LOG_INFO("Initializing OGC WMS...");

  manager::instance().initialize();

  std::unique_ptr<handler> h(new handler);
  eows::core::service_operations_manager::instance().insert("/wms", std::move(h));

  EOWS_LOG_INFO("OGC WMS service initialized!");
}

void
eows::ogc::wms::return_exception(const char* msg, eows::core::http_response& res)
{
  xml_document<> xml_doc;

  xml_node<>* decl = xml_doc.allocate_node(node_declaration);
  decl->append_attribute(xml_doc.allocate_attribute("version", "1.0"));
  decl->append_attribute(xml_doc.allocate_attribute("encoding", "UTF-8"));
  xml_doc.append_node(decl);

  xml_node<>* service_exception_report = xml_doc.allocate_node(node_element, "ServiceExceptionReport");
  service_exception_report->append_attribute(xml_doc.allocate_attribute("version","1.3.0"));
  service_exception_report->append_attribute(xml_doc.allocate_attribute("xmlns","http://www.opengis.net/ogc"));
  service_exception_report->append_attribute(xml_doc.allocate_attribute("xmlns:xsi","http://www.w3c.org/2001/XMLSchema-instance"));
  service_exception_report->append_attribute(xml_doc.allocate_attribute("xsi:schemaLocation","http://www.opengis.net/ogc http://schemas.opengis.net/wms/1.3.0/exceptions_1_3_0.xsd"));
  xml_doc.append_node(service_exception_report);

  xml_node<>* service_exception = xml_doc.allocate_node(node_element, "ServiceException");
  service_exception->append_attribute(xml_doc.allocate_attribute("code","WMSError"));
  service_exception->append_node(xml_doc.allocate_node(node_cdata, nullptr, msg));
  service_exception_report->append_node(service_exception);

  std::string buff;

  rapidxml::print(std::back_inserter(buff), xml_doc, 0);

  res.set_status(eows::core::http_response::OK);

  res.add_header(eows::core::http_response::CONTENT_TYPE, "text/xml; charset=utf-8");
  res.add_header(eows::core::http_response::ACCESS_CONTROL_ALLOW_ORIGIN, "*");

  res.write(buff.c_str(), buff.length());
}
