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
#include <ctime>
#include <vector>

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

    //! Split a query string into its components (the query string must be hex encoded).
    /*!
      \exception eows::parse_error Throws an exception if the query string can not be parsed.
     */
    eows::core::query_string_t expand(const std::string& query_str);

    /*!
     * \brief Retrieves a HTTP Referer from request. Used to redirect next uri
     *
     * The Referer request header contains the URI of the previous web page, allowing servers to
     * identify where people are visiting them FROM.
     *
     * See more in https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Referer
     *
     * \todo It must be moved to eows::core since it belongs to HTTP Request and there is no special behavior for retrieve URI handling.
     *
     * \param request
     * \return
     */
    const std::string referer(const eows::core::http_request& request);

    /*!
     * \brief Retrieves a HTTP Authozation header from request.
     *
     * \throws eows::parse_error When could not find Authorization header in HTTP Request
     *
     * \param request - HTTP Request
     * \return Parsed HTTP Authorization header
     */
    authorization_t authorization(const eows::core::http_request& request);

    /*!
     * \brief Parses cookies from HTTP Request Header
     *
     * A cookie HTTP request header contains stored HTTP cookies previously sent by the
     * server with the Set-Cookie
     *
     * Further informataion in https://developer.mozilla.org/pt-BR/docs/Web/HTTP/Headers/Cookie
     *
     * \param request - HTTP Request Header
     * \return Map information containing HTTP Cookies
     */
    std::map<std::string, std::string> parse_cookies(const eows::core::http_request& request);

    /*!
     * \brief Transforms a Query String parameters into string representation
     *
     * \example
     * eows::core::query_string_t q;
     * q.insert(std::make_pair("foo", "bar"));
     * q.insert(std::make_pair("foo2", "bar2"));
     *
     * std::cout << eows::core::to_str(q) << std::endl;
     * > ?foo=bar&foo2=bar2
     *
     * \param query_string - HTTP Query String
     *
     * \return String representation of query parameters
     */
    const std::string to_str(const query_string_t& query_string);

    template<class output_type_t, class iterator_t>
    output_type_t join(const iterator_t& begin, const iterator_t& end, const output_type_t& t);

    /*!
     * \brief Applies ltrim and rtrim operation
     *
     * \param str - String value
     *
     * \return String value without empty chars from left and right side
     */
    std::string trim(const std::string& str);

    /*!
     * \brief Performs string split and transforms it in array
     *
     * \param str - String value to split
     * \param delimiter - Delimiter to apply
     * \param roles - Array output
     */
    void split(const std::string& str, char delimiter, std::vector<std::string>& roles);

    /*!
     * \brief Retrieves unix timestamp value.
     *
     * \param t - Time input. Default is now
     *
     * \return Unix timestamp
     */
    uint64_t unix_timestamp(std::time_t* t = nullptr);

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
    double read_node_as_double(const rapidjson::Value& node, const std::string& member_name);

    /*!
     * \brief Tries to find a member by name in RapidJSON Node and then read it as double.
     * \throws eows::parse_error When could not read or process as double
     * \param node - RapidJSON node
     * \return Value double
     */
    double read_node_as_double(const rapidjson::Value& node);

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
     * \brief It generates a unique path (temporary) in system.
     *
     * It uses app_settings temporary data directory as prefix.
     *
     * \return A Temp file name
     */
    std::string generate_unique_path(const std::string& extension = "");

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

template<class output_type_t, class iterator_t>
output_type_t eows::core::join(const iterator_t& begin, const iterator_t& end, const output_type_t& t)
{
  output_type_t result;

  for(iterator_t it = begin; it != end; ++it)
  {
    if (!result.empty())
      result.append(t);
    result.append(*it);
  }

  return result;
}

#endif  // __EOWS_CORE_UTILS_HPP__

