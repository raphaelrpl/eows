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
  \file eows/core/utils.cpp

  \brief General utility functions.

  \author Gilberto Ribeiro de Queiroz
 */

// EOWS
#include "utils.hpp"
#include "app_settings.hpp"
#include "defines.hpp"
#include "exception.hpp"
#include "http_request.hpp"
#include "logger.hpp"
#include "web_service_handler.hpp"

// STL
#include <fstream>

// Boost
#include <boost/log/attributes/current_thread_id.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/trivial.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

// RapiJSON
#include <rapidjson/istreamwrapper.h>

BOOST_LOG_ATTRIBUTE_KEYWORD(channel, "Channel", std::string)


void
eows::core::initialize()
{
// Find out the log file name
  const rapidjson::Document& doc = app_settings::instance().get();

  rapidjson::Value::ConstMemberIterator jlog_file = doc.FindMember("log_file");

  if((jlog_file == doc.MemberEnd()) || (!jlog_file->value.IsString()))
    throw eows::parse_error("Please check key 'log_file' in file: \"" EOWS_CONFIG_FILE "\".");

  const std::string log_file_name = jlog_file->value.GetString();
  
// Prepare log format
  boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");

  boost::log::core::get()->add_global_attribute("ThreadID", boost::log::attributes::current_thread_id());

  boost::log::add_common_attributes();
  
  const std::string format = "(%ThreadID%) [%TimeStamp%] <%Severity%>: %Message%";
  const std::string channel_name = "eows";
  
  boost::log::add_file_log(boost::log::keywords::auto_flush = true,
                           boost::log::keywords::format = format,
                           boost::log::keywords::file_name = log_file_name,
                           boost::log::keywords::rotation_size = 10 * 1024 * 1024,
                           boost::log::keywords::scan_method = boost::log::sinks::file::scan_matching,
                           //boost::log::keywords::open_mode = std::ios_base::app,
                           boost::log::keywords::channel = channel_name,
                           boost::log::keywords::filter = (channel == channel_name)
                          );

  EOWS_LOG_INFO("EOWS core runtime initialized!");
}

std::pair<std::string, std::string>
eows::core::split_path_and_query_str(const std::string& str)
{
  std::string::size_type pos = str.find("?");

// first is path
// second is query_str
  std::pair<std::string, std::string> result;

  if(pos != std::string::npos)
  {
    result.first = str.substr(0, pos);
    result.second = str.substr(pos + 1);
  }
  else
  {
    result.first = str;
  }
      
  return result;
}

static const char application_json_[] = "application/json",
                  application_xml_[] = "application/xml",
                  image_gif_[] = "image/gif",
                  image_jpeg_[] = "image/jpeg",
                  image_png_[] = "image/png",
                  image_tiff_[] = "image/tiff",
                  image_x_tiff_[] = "image/x-tiff",
                  text_plain_[] = "text/plain",
                  text_xml_[] = "text/xml",
                  text_html_[] = "text/html",
                  unknown_[] = "unknown";

const char*
eows::core::to_str(content_type_t content_type)
{
  switch(content_type)
  {
    case APPLICATION_JSON:
      return application_json_;
    case APPLICATION_XML:
      return application_xml_;
    case IMAGE_GIF:
      return image_gif_;
    case IMAGE_JPEG:
      return image_jpeg_;
    case IMAGE_PNG:
      return image_png_;
    case IMAGE_TIFF:
      return image_tiff_;
    case IMAGE_X_TIFF:
      return image_x_tiff_;
    case TEXT_PLAIN:
      return text_plain_;
    case TEXT_XML:
      return text_xml_;
    case TEXT_HTML:
      return text_html_;
    default:
      return unknown_;
  }
}

void
eows::core::process(web_service_handler& handler,
                    const http_request& request,
                    http_response& response)
{
  if(request.method() == "GET")
    handler.do_get(request, response);
  else if(request.method() == "POST")
    handler.do_post(request, response);
  else if(request.method() == "HEAD")
    handler.do_head(request, response);
  else if(request.method() == "PUT")
    handler.do_put(request, response);
  else if(request.method() == "DELETE")
    handler.do_delete(request, response);
  else if(request.method() == "TRACE")
    handler.do_trace(request, response);
  else if(request.method() == "CONNECT")
    handler.do_connect(request, response);
  else if(request.method() == "OPTIONS")
    handler.do_options(request, response);
  else
  {
    boost::format err_msg("Invalid request method: %1%.");

    throw std::logic_error((err_msg % request.method()).str());
  }
}

rapidjson::Document
eows::core::open_json_file(const std::string& path)
{
  if(!boost::filesystem::is_regular(path))
  {
    boost::format err_msg("Input file '%1%' doesn't exist.");

    throw std::invalid_argument((err_msg % path).str());
  }

  std::ifstream file(path.c_str(), std::ifstream::in);

//  Note: RapidJSON will not accept working with exceptions!
//
//  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
//  
//  try
//  {
//    file.open(path.c_str(), std::ifstream::in);
//  }
//  catch(const std::ifstream::failure& e)
//  {
//    boost::format err_msg("Could not open file: '%1%'.");
//  
//    throw std::invalid_argument((err_msg % path).str());
//  }
  
  rapidjson::Document doc;
  
  try
  {
    rapidjson::IStreamWrapper istr(file);
    
    doc.ParseStream(istr);
    
    if(doc.HasParseError())
    {
      boost::format err_msg("Error parsing input file '%1%': %2%.");
      
      throw eows::parse_error((err_msg % path % doc.GetParseError()).str());
    }
    
    if(!doc.IsObject() || doc.IsNull())
    {
      boost::format err_msg("Error parsing input file '%1%': unexpected file format.");

      throw eows::parse_error((err_msg % path).str());
    }

    file.close();
  }
  catch(...)
  {
    file.close();
    throw;
  }
  
  return doc;
}

const std::string eows::core::read_node_as_string(const rapidjson::Value& node, const std::string& member_name)
{
  rapidjson::Value::ConstMemberIterator jit = node.FindMember(member_name.c_str());
  // TODO: auto format function in common
  if((jit == node.MemberEnd()) || (!jit->value.IsString()))
    throw eows::parse_error("Please, check the key " + member_name + " in JSON document.");
  return jit->value.GetString();
}


std::map<std::string, std::string> eows::core::lowerify(const std::map<std::string, std::string>& given)
{
  std::map<std::string, std::string> out;

  for(auto& it: given)
  {
    out.insert(std::pair<std::string, std::string>(to_lower(it.first), it.second));
  }

  return out;
}

std::string eows::core::to_lower(const std::string& str)
{
  std::string out;
  out.resize(str.size());
  std::transform(str.begin(), str.end(), out.begin(), ::tolower);

  return out;
}
