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
  \file eows/core/utils.hpp

  \brief General utility functions.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_CORE_UTILS_HPP__
#define __EOWS_CORE_UTILS_HPP__

// EOWS
#include "data_types.hpp"
#include "http_response.hpp"

// STL
#include <algorithm>
#include <iterator>
#include <map>
#include <string>
#include <utility>

// RapidJSON
#include <rapidjson/document.h>

namespace eows
{
  namespace core
  {
    class http_request;
    class http_response;
    class web_service_handler;
    
    /*!
      \exception std::exception ...
     */
    void initialize();

    const std::string to_str(const query_string_t& query_string);

    /**
     * \brief Tries to find a member by name in RapidJSON Node and then read it as boolean.
     * \throws eows::parse_error When could not find member name or process as boolean like
     * \return Bool value of node
     */
    bool read_node_as_bool(const rapidjson::Value& node, const std::string& member_name);

    /*!
     * \brief Tries to read RapidJSON Node as boolean value.
     * \throws eows::parse_error When node is not boolean readble
     * \return bool value of node
     */
    bool read_node_as_bool(const rapidjson::Value& node);

    /**
     * \brief Tries to find a member by name in RapidJSON Node and then read it as string.
     * \throws eows::parse_error When could not find member name or process as string like
     * \return String value of node
     */
    const std::string read_node_as_string(const rapidjson::Value&, const std::string&);

    /*!
     * \brief Tries to read RapidJSON Node as string value.
     * \throws eows::parse_error When node is not string readble
     * \return String value of node
     */
    const std::string read_node_as_string(const rapidjson::Value& node);

    /*!
     * \brief Tries to find a member by name in RapidJSON Node and then read it as int64.
     * \throws eows::parse_error When could not read or process as int64_t
     * \param node - RapidJSON node
     * \param member_name - Member to find in node
     * \return Value int64
     */
    int64_t read_node_as_int64(const rapidjson::Value& node, const std::string& member_name);

    /*!
     * \brief Tries to find a member by name in RapidJSON Node and then read it as int64_t.
     * \throws eows::parse_error When could not read or process as int64_t
     * \param node - RapidJSON node
     * \return Value int64_t
     */
    int64_t read_node_as_int64(const rapidjson::Value& node);

    /*!
     * \brief Tries to find a member by name in RapidJSON Node and then read it as double.
     * \throws eows::parse_error When could not read or process as double
     * \param node - RapidJSON node
     * \param member_name - Member to find in node
     * \return Value double
     */
    int64_t read_node_as_double(const rapidjson::Value& node, const std::string& member_name);

    /*!
     * \brief Tries to find a member by name in RapidJSON Node and then read it as double.
     * \throws eows::parse_error When could not read or process as double
     * \param node - RapidJSON node
     * \return Value double
     */
    int64_t read_node_as_double(const rapidjson::Value& node);

    /**
     * \brief It transforms a string into lowercase
     * \return String in lowercase
     */
    std::string to_lower(const std::string&);

    /*!
     * \brief It tries to decode a string value
     * \param encoded_string - Encoded String
     * \return Decoded string
     */
    std::string decode(const std::string& encoded_string);

    /*!
     * \brief Decodes a string in base64 format
     * \param encoded_string Encoded b64 string
     * \return decoded string
     */
    std::string base64_decode(const std::string& encoded_string);

    /*!
     * \brief It generates a unique path (temporary) in system.
     *
     * It uses app_settings temporary data directory as prefix.
     *
     * \return A Temp file name
     */
    std::string generate_unique_path(const std::string& extension = "");

    /*!
     * \brief Utility for render a json response for HTTP requests
     * \param response HTTP Response object
     * \param status_code HTTP status code
     * \param json JSON message to reply
     */
    void response_json(http_response& response, http_response::status_t status_code, const std::string& json);

    /**
     * \brief It applies lower case on given map keys and return a new map with these values
     * \return Copy map with keys in lowercase
     */
    std::multimap<std::string, std::string> lowerify(const std::multimap<std::string, std::string>&);
    
    //! Split string into path and query-string
    std::pair<std::string, std::string>
    split_path_and_query_str(const std::string& str);

    /*! \brief Returns the string representation for a give content type. */
    const char* to_str(content_type_t content_type);
    content_type_t from_string(const std::string& content);
    
    /*!
      \brief Find out the righ request-method (GET, PUT, POST) and and process the request.
     
      \param handler The handle to be used to process the request.
      \param request The client request.
      \param reponse The server response.

      \exception std::logic_errror Throws an exception if the request method is unknown.
     */
    void process(web_service_handler& handler,
                 const http_request& request,
                 http_response& response);
    
    /*!
      \exception std::invalid_argument If the file can not be opened.
      
      \exception eows::parse_error If the JSON file is invalid.
     */
    rapidjson::Document open_json_file(const std::string &path);

    //! Write a string sequence as a JSON array.
    template<class InputIterator, class Writer> void
    write_string_array(InputIterator first,
                       InputIterator last,
                       Writer& writer);

    //! Copy a numeric sequence to a JSON array
    template<class InputIterator, class Writer> void
    write_numeric_array(InputIterator first, InputIterator last, Writer& w);

    //! Copy the JSON string array to a range beginning at result.
    template<class OutputIterator> void
    copy_string_array(const rapidjson::Value& jvalues, OutputIterator result);

  }  // end namespace core
}    // end namespace eows

template<class InputIterator, class Writer> inline void
eows::core::write_string_array(InputIterator first, InputIterator last, Writer& w)
{
  w.StartArray();

  std::for_each(first, last, [&w](const std::string& v) -> void
                             { w.String(v.c_str(), static_cast<rapidjson::SizeType>(v.length())); });

  w.EndArray();
}

template<class InputIterator, class Writer> inline void
eows::core::write_numeric_array(InputIterator first, InputIterator last, Writer& w)
{
  w.StartArray();

  std::for_each(first, last, [&w](typename std::iterator_traits<InputIterator>::value_type v) -> void
                             { w.Double(v); });

  w.EndArray();
}

template<class OutputIterator> inline void
eows::core::copy_string_array(const rapidjson::Value& jvalues,
                              OutputIterator result)
{
  if(!jvalues.IsArray())
    throw std::invalid_argument("copy_string_array: jvalues argument must be a JSON array.");

  if(jvalues.Empty())
    return;

  const rapidjson::SizeType nelements = jvalues.Size();

  for(rapidjson::SizeType i = 0; i != nelements; ++i)
  {
    const rapidjson::Value& jelement = jvalues[i];

    if(!jelement.IsString())
      throw std::invalid_argument("copy_string_array: only string elements are allowed in array.");

    if(jelement.IsNull())
      continue;

    *result++ = jelement.GetString();
  }
}

#endif  // __EOWS_CORE_UTILS_HPP__

