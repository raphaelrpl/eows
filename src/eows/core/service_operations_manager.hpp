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
  \file eows/core/service_operations_manager.hpp

  \brief A singleton for registering service operations.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_CORE_SERVICE_OPERATIONS_MANAGER_HPP__
#define __EOWS_CORE_SERVICE_OPERATIONS_MANAGER_HPP__

// STL
#include <map>
#include <memory>
#include <string>
#include <vector>

// Boost
#include <boost/noncopyable.hpp>

namespace eows
{
  namespace core
  {

// Forward declaration
    class http_request;
    class http_response;
    class web_service_handler;

    //! A singleton for managing service operations.
    class service_operations_manager : public boost::noncopyable
    {
      public:
      
        /*!
          \brief Returns a service operation associated to the given path.
          
          \param path A valid service path.

          \exception std::out_of_range Throws an exception if there is no service operation associated to the path.
         */
        web_service_handler* get(const std::string& path) const;

        /*!
          \brief Register a new web service handler associated to the path.

          \exception std::invalid_argument Throws an exception if a service operation with the same path is already registered.
         */
        void insert(const std::string& path, std::unique_ptr<web_service_handler> handler);
      
                void process(const http_request& request,
                     http_response& response);

        //! Access the singleton.
        static service_operations_manager& instance();
      
        //! Returns the list of all registered paths.
        std::vector<std::string> registered_paths() const;

      protected:

        //! Constructor.
        service_operations_manager() = default;

        //! Destructor.
        ~service_operations_manager() = default;

        //! Returns true if a given path is already in use.
        bool exists(const std::string& path) const
        {
          return operations_idx_.find(path) != operations_idx_.end();
        }

      private:

        std::map<std::string, std::unique_ptr<web_service_handler> > operations_idx_;  //!< An index for service operations.
    };

  }   // end namespace core
}     // end namespace eows

#endif  // __EOWS_CORE_SERVICE_OPERATIONS_MANAGER_HPP__
