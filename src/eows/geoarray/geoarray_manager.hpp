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
  \file eows/geoarray/geoarray_manager.hpp

  \brief A singleton for managing the registered geo-arrays.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_GEOARRAY_GEOARRAY_MANAGER_HPP__
#define __EOWS_GEOARRAY_GEOARRAY_MANAGER_HPP__

// STL
#include <vector>
#include <string>

// Boost
#include <boost/noncopyable.hpp>

namespace eows
{
  namespace geoarray
  {
  
    //! Forward declaration
    struct geoarray_t;

    /*!
      \class geoarray_manager
      
      \brief A singleton for managing geo-arrays.
      
      The identifier of a GeoArray is composed by its name and
      the name of its cluster. For instance, an array
      named mod13q1 stored in a cluster named chronos will
      be named: chronos:mod13q1.
     */
    class geoarray_manager : public boost::noncopyable
    {
      public:

        /*!
          \exception std::invalid_argument If another array with the same name and data-source is already registered.
         */
        void insert(const geoarray_t& a);

        //! Returns the list of GeoArrays.
        std::vector<std::string> list_arrays() const;

        /*!
          \exception std::invalid_argument If no array is found.
         */
        const geoarray_t& get(const std::string& array_name) const;

        static geoarray_manager& instance();

      private:

        geoarray_manager();

        ~geoarray_manager();

      private:

        struct impl;

        impl* pimpl_;
    };

  }  // end namespace geoarray
}    // end namespace eows

#endif  // __EOWS_GEOARRAY_GEOARRAY_MANAGER_HPP__
