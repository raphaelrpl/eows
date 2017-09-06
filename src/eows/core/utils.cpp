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
// Boost Base64
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/algorithm/string.hpp>

// RapiJSON
#include <rapidjson/istreamwrapper.h>

BOOST_LOG_ATTRIBUTE_KEYWORD(channel, "Channel", std::string)


void
eows::core::initialize()
{
// Find out the log file name and temporary data directory
  const rapidjson::Document& doc = app_settings::instance().get();

  const std::string log_file_name = read_node_as_string(doc, "log_file");
  
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

  rapidjson::Value::ConstMemberIterator temp_data_it = doc.FindMember("tmp_data_dir");

  if(temp_data_it == doc.MemberEnd() || !temp_data_it->value.IsString())
    throw eows::eows_error("The temporary data dir was not set int '" EOWS_CONFIG_FILE "' configuration file. Please, check the 'tmp_data_dir' key entry");

  std::string temp_data_dir = read_node_as_string(temp_data_it->value);

  if(!boost::filesystem::exists(temp_data_dir))
  {
    boost::format msg("The temporary data directory '%1%' does not exist. It will be created.");
    EOWS_LOG_INFO((msg % temp_data_dir).str());

    if(!boost::filesystem::create_directories(temp_data_dir))
    {
      boost::format err_msg("Could not create temporary data directory: '%1%'.");
      throw eows::eows_error((err_msg % temp_data_dir).str());
    }
    else
    {
      boost::format msg("Temporary data directory '%1%' created successfully.");
      EOWS_LOG_INFO((msg % temp_data_dir).str());
    }
  }
  else if(!boost::filesystem::is_directory(temp_data_dir))
  {
    boost::format err_msg("The informed temporary data directory '%1%' is not a regular directory.");
    EOWS_LOG_INFO((err_msg % temp_data_dir).str());
    throw eows::eows_error((err_msg % temp_data_dir).str());
  }

// Setting temporary data directory
  app_settings::instance().set_tmp_data_dir(temp_data_dir);

  boost::format msg("Using temporary data directory: %1%.");
  EOWS_LOG_INFO((msg % temp_data_dir).str());
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
                  application_gml_xml[] = "application/gml+xml",
                  image_gif_[] = "image/gif",
                  image_jpeg_[] = "image/jpeg",
                  image_png_[] = "image/png",
                  image_tiff_[] = "image/tiff",
                  image_x_tiff_[] = "image/x-tiff",
                  text_plain_[] = "text/plain",
                  text_xml_[] = "text/xml",
                  text_html_[] = "text/html",
                  unknown_[] = "application/octet-stream";

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
  if((jit == node.MemberEnd()))
    throw eows::parse_error("Please, check the key " + member_name + " in JSON document.");
  return read_node_as_string(jit->value);
}

const std::string eows::core::read_node_as_string(const rapidjson::Value& node)
{
  if (!node.IsString())
    throw eows::parse_error("Could not read JSON node as string");
  return node.GetString();
}

int64_t eows::core::read_node_as_int64(const rapidjson::Value& node, const std::string& member_name)
{
  rapidjson::Value::ConstMemberIterator jit = node.FindMember(member_name.c_str());
  // TODO: auto format function in common
  if((jit == node.MemberEnd()))
    throw eows::parse_error("Please, check the key " + member_name + " in JSON document.");
  return read_node_as_int64(jit->value);
}

int64_t eows::core::read_node_as_int64(const rapidjson::Value& node)
{
  if (!node.IsInt64())
    throw eows::parse_error("Could not read JSON node as int64_t");
  return node.GetInt64();
}

int64_t eows::core::read_node_as_double(const rapidjson::Value& node, const std::string& member_name)
{
  rapidjson::Value::ConstMemberIterator jit = node.FindMember(member_name.c_str());

  if(jit == node.MemberEnd())
      throw eows::parse_error("Please, check the key " + member_name + " in JSON document.");
  return read_node_as_double(jit->value);
}

int64_t eows::core::read_node_as_double(const rapidjson::Value& node)
{
  if (!node.IsNumber())
    throw eows::parse_error("Could not read JSON node as number");
  return node.GetDouble();
}

std::multimap<std::string, std::string> eows::core::lowerify(const std::multimap<std::string, std::string>& given)
{
  std::multimap<std::string, std::string> out;

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

std::string eows::core::generate_unique_path(const std::string& extension)
{
  return app_settings::instance().get_tmp_data_dir() + boost::filesystem::unique_path().string() + extension;
}

eows::core::content_type_t eows::core::from_string(const std::string& content)
{
  if (content == application_xml_ ||
      content == application_gml_xml)
    return APPLICATION_XML;
  if (content == application_json_)
    return APPLICATION_JSON;
  if (content == image_gif_)
    return IMAGE_GIF;
  if (content == image_jpeg_)
    return IMAGE_JPEG;
  if (content == image_png_)
    return IMAGE_PNG;
  if (content == image_tiff_)
    return IMAGE_TIFF;
  if (content == image_x_tiff_)
    return IMAGE_X_TIFF;
  if (content == text_html_)
    return TEXT_HTML;
  if (content == text_plain_)
    return TEXT_PLAIN;
  if (content == text_xml_)
    return TEXT_XML;
  return APPLICATION_OCTET_STREAM; // Default/unknown/no extension format
}

std::string eows::core::decode(const std::string& encoded_string)
{
  std::string output;

  int tmp;
  for(std::size_t i = 0; i < encoded_string.size(); ++i)
  {
    if (encoded_string[i] == '%')
    {
      std::string t = encoded_string.substr(i + 1, 2);
      std::stringstream ss(t);
      ss >> std::hex >> tmp;
      output += tmp;
      i = i + 2;
    }
    else
      output += encoded_string[i];
  }
  return output;
}

void eows::core::response_json(eows::core::http_response& response, http_response::status_t status_code, const std::string& json)
{
  response.set_status(status_code);
  response.add_header(eows::core::http_response::CONTENT_TYPE, "application/json; charset=utf-8");
  response.write(json.c_str(), json.size());
}

std::string eows::core::base64_decode(const std::string& encoded_string)
{
  using namespace boost::archive::iterators;
  using It = transform_width<binary_from_base64<std::string::const_iterator>, 8, 6>;
  return boost::algorithm::trim_right_copy_if(std::string(It(std::begin(encoded_string)),
                                                          It(std::end(encoded_string))),
                                                         [](const char c) {
                                                           return c == '\0';
  });
}
bool eows::core::read_node_as_bool(const rapidjson::Value&node, const std::string&member_name)
{
  rapidjson::Value::ConstMemberIterator jit = node.FindMember(member_name.c_str());
  // TODO: auto format function in common
  if((jit == node.MemberEnd()))
    throw eows::parse_error("Please, check the key " + member_name + " in JSON document.");
  return read_node_as_bool(jit->value);
}

bool eows::core::read_node_as_bool(const rapidjson::Value& node)
{
  if (!node.IsBool())
    throw eows::parse_error("Could not read JSON node as bool");
  return node.GetBool();
}

const std::string eows::core::to_str(const eows::core::query_string_t& query_string)
{
  std::string output("?");
  for(const auto it: query_string)
    output.append(it.first + "=" + it.second + "&");
  if (output[output.size()-1] == '&')
    output.pop_back();
  return output;
}

std::string eows::core::base64_encode(const std::string& raw_string)
{
  using namespace boost::archive::iterators;
  using It = base64_from_binary<transform_width<std::string::const_iterator, 6, 8>>;
  auto tmp = std::string(It(std::begin(raw_string)), It(std::end(raw_string)));
  return tmp.append((3 - raw_string.size() % 3) % 3, '=');
}

std::vector<std::string> eows::core::split(const std::string& str, char delimiter, std::vector<std::string>& roles)
{
  boost::split(roles, str, boost::is_any_of("\t"+delimiter));

  return roles;
}
