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
  \file eows/core/http_response.hpp

  \brief Base class that models an HTTP response.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_CORE_HTTP_RESPONSE_HPP__
#define __EOWS_CORE_HTTP_RESPONSE_HPP__

// STL
#include <string>

// Boost
#include <boost/noncopyable.hpp>

namespace eows
{
  namespace core
  {

    //! Base class that models an HTTP response.
    class http_response : public boost::noncopyable
    {
      public:
      
        /*!
          \enum status_t
          
          \brief The set of known status codes for HTTP server responses.
          
          \note See https://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html for a brief explanation of status code.
         */
        enum status_t
        {
          OK = 200,
          created = 201,
          accepted = 202,
          no_content = 204,
          partial_content = 206,
          multiple_choices = 300,
          moved_permanently = 301,
          not_modified = 304,
          bad_request = 400,
          unauthorized = 401,
          forbidden = 403,
          not_found = 404,
          NOT_SUPPORTED = 405, /* TODO: 405 => Method Not Allowed !!!! */
          not_acceptable = 406,
          request_timeout = 408,
          precondition_failed = 412,
          unsatisfiable_range = 416,
          internal_server_error = 500,
          not_implemented = 501,
          bad_gateway = 502,
          service_unavailable = 503,
          space_unavailable = 507
        };
      
        /*!
          \enum header_type_t
      
          \brief The response fields (also known as media-type or MIME-type) defines the type format contents transmitted between HTTP server and client.
         */
        enum header_type_t
        {
          ACCESS_CONTROL_ALLOW_ORIGIN,
          CONTENT_LENGTH,
          CONTENT_TYPE,
          LOCATION
        };

        //! Constructor.
        http_response() = default;

        //! Virtual destructor.
        virtual ~http_response() = default;
      
        /*!
          \brief Sets the status code of the response.
         
          \pre Headers have not been set.
         
          \post Status is set on the response.
         
          \throw std::logic_error when the precondition is violated.
         */
        virtual void set_status(status_t new_status) = 0;

        /*!
          \brief Add a well-known header to the response.

          \pre Data have not been write.

          \post The header is added to the response.

          \throw std::logic_error when the precondition is violated.
         */
        virtual void add_header(header_type_t field_name, const std::string& value) = 0;
      
        /*!
          \brief Add a free-header to the response.

          \pre Data have not been write.

          \post The header is added to the response.

          \throw std::logic_error when the precondition is violated.
         */
        virtual void add_header(const std::string& field_name, const std::string& value) = 0;

        /*!
          \brief Write data to the response.

          \note The implementation must not cache the pointer.
         */
        virtual void write(const char* value, const std::size_t size) = 0;
      
      protected:
    
        static const char access_control_allow_origin_[];
        static const char content_length_[];
        static const char content_type_[];
        static const char location_[];
        static const char unknown_[];
      
        static const char * to_str(header_type_t field_name)
        {
          switch(field_name)
          {
            case ACCESS_CONTROL_ALLOW_ORIGIN:
              return access_control_allow_origin_;
            case CONTENT_LENGTH:
              return content_length_;
            case CONTENT_TYPE:
              return content_type_;
            case LOCATION:
              return location_;
            default:
              return unknown_;
          }
        }

//      constexpr static char const ok_[] = "OK",
//                             created_[] = "Created",
//                            accepted_[] = "Accepted",
//                          no_content_[] = "No Content",
//                     partial_content_[] = "Partial Content",
//                    multiple_choices_[] = "Multiple Choices",
//                   moved_permanently_[] = "Moved Permanently",
//                        not_modified_[] = "Not Modified",
//                         bad_request_[] = "Bad Request",
//                        unauthorized_[] = "Unauthorized",
//                           forbidden_[] = "Fobidden",
//                           not_found_[] = "Not Found",
//                       not_supported_[] = "Not Supported",
//                      not_acceptable_[] = "Not Acceptable",
//                     request_timeout_[] = "Request Timeout",
//                 precondition_failed_[] = "Precondition Failed",
//                 unsatisfiable_range_[] = "Requested Range Not Satisfiable",
//               internal_server_error_[] = "Internal Server Error",
//                     not_implemented_[] = "Not Implemented",
//                         bad_gateway_[] = "Bad Gateway",
//                 service_unavailable_[] = "Service Unavailable",
//                   space_unavailable_[] = "Insufficient Space to Store Resource",
//                             unknown_[] = "Unknown";

//      static char const* status_message(status_t status)
//      {
//        switch (status)
//        {
//          case ok: return ok_;
//          case created: return created_;
//          case accepted: return accepted_;
//          case no_content: return no_content_;
//          case multiple_choices: return multiple_choices_;
//          case moved_permanently: return moved_permanently_;
//          case moved_temporarily: return moved_temporarily_;
//          case not_modified: return not_modified_;
//          case bad_request: return bad_request_;
//          case unauthorized: return unauthorized_;
//          case forbidden: return forbidden_;
//          case not_found: return not_found_;
//      case not_supported:
//        return not_supported_;
//      case not_acceptable:
//        return not_acceptable_;
//      case internal_server_error:
//        return internal_server_error_;
//      case not_implemented:
//        return not_implemented_;
//      case bad_gateway:
//        return bad_gateway_;
//      case service_unavailable:
//        return service_unavailable_;
//      case partial_content:
//        return partial_content_;
//      case request_timeout:
//        return request_timeout_;
//      case precondition_failed:
//        return precondition_failed_;
//      case unsatisfiable_range:
//        return unsatisfiable_range_;
//      case space_unavailable:
//        return space_unavailable_;
//      default: return unknown_;
//        }
//  }

    };

  }   // end namespace core
}     // end namespace eows

#endif  // __EOWS_CORE_HTTP_RESPONSE_HPP__
