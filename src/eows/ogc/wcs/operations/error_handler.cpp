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
  \file eows/ogc/wcs/operations/error_handler.cpp

  \brief Represents implementation of WCS exception handler

  \author Raphael Willian da Costa
 */


// EOWS
#include "error_handler.hpp"
#include "../exception.hpp"
// RapidXML
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>

const std::string eows::ogc::wcs::operations::handle_error(const ogc_error& e)
{
  rapidxml::xml_document<> xml_doc;
  // Preparing Meta document
  rapidxml::xml_node<>* decl = xml_doc.allocate_node(rapidxml::node_declaration);
  decl->append_attribute(xml_doc.allocate_attribute("version", "1.0"));
  decl->append_attribute(xml_doc.allocate_attribute("encoding", "UTF-8"));
  xml_doc.append_node(decl);
  // Preparing WCS Root element
  rapidxml::xml_node<>* root = xml_doc.allocate_node(rapidxml::node_element, "ows:ExceptionReport");
  root->append_attribute(xml_doc.allocate_attribute("version", "2.0.0"));
  root->append_attribute(xml_doc.allocate_attribute("xmlns","http://www.opengis.net/ows/2.0"));
  root->append_attribute(xml_doc.allocate_attribute("xmlns:ows","http://www.opengis.net/ows/2.0"));
  root->append_attribute(xml_doc.allocate_attribute("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance"));
  root->append_attribute(xml_doc.allocate_attribute("xsi:schemaLocation","http://www.opengis.net/ows/2.0 http://schemas.opengis.net/ows/2.0/owsExceptionReport.xsd"));
  xml_doc.append_node(root);

  rapidxml::xml_node<>* exception = xml_doc.allocate_node(rapidxml::node_element, "ows:Exception");
  rapidxml::xml_attribute<>* attr = xml_doc.allocate_attribute("exceptionCode", e.error_code.c_str());
  exception->append_attribute(attr);
  attr = xml_doc.allocate_attribute("locator", "");
  exception->append_attribute(attr);
  // Preparing WCS exception text
  rapidxml::xml_node<>* exception_text = xml_doc.allocate_node(rapidxml::node_element, "ows:ExceptionText", e.what());
  exception->append_node(exception_text);
  root->append_node(exception);

  std::string buff;
  rapidxml::print(std::back_inserter(buff), xml_doc, 0);
  return buff;
}
