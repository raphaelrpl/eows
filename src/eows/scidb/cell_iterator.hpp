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
  \file eows/scidb/cell_iterator.hpp

  \brief A cell iterator for a multidimensional array with several attributes.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_SCIDB_CELL_ITERATOR_HPP__
#define __EOWS_SCIDB_CELL_ITERATOR_HPP__

// STL
#include <memory>
#include <string>

// Boost
#include <boost/noncopyable.hpp>

// SciDB-API
#include <SciDBAPI.h>

namespace eows
{
  namespace scidb
  {

    //! A cell iterator for a multidimensional array with several attributes.
    class cell_iterator : public boost::noncopyable
    {
      public:

        //! Constructor.
        cell_iterator(const std::shared_ptr< ::scidb::Array >& a);

        //! Destructor.
        ~cell_iterator();

        //!
        const ::scidb::Coordinates& get_position();

        //! Returns a string value for the attribute indicated by a given position.
        //std::string get_str(const std::size_t attr_pos) const;

        //! Returns a string value for the attribute indicated by a given name.
        //std::string get_str(const std::string& attr_name) const;

        //! Returns a float value for the attribute indicated by a given position.
        //double get_double(const std::size_t attr_pos) const;

        //! Returns a float value for the attribute indicated by a given name.
        //double get_double(const std::string& attr_name) const;

        //! Returns a 64-bit integer for the attribute indicated by a given position.
        //int64_t get_int64(const std::size_t attr_pos) const;

        //! Returns a 64-bit integer for the attribute indicated by a given name.
        //int64_t get_int64(const std::string& attr_name) const;

        //! Returns a 64-bit unsigned integer for the attribute indicated by a given position.
        //uint64_t get_uint64(const std::size_t attr_pos) const;

        //! Returns a 64-bit unsigned integer for the attribute indicated by a given name.
        //uint64_t get_uint64(const std::string& attr_name) const;

        //! Returns a 32-bit unsigned integer for the attribute indicated by a given position.
        //uint32_t get_uint32(const std::size_t attr_pos) const;

        //! Returns a 32-bit unsigned integer for the attribute indicated by a given name.
        //uint32_t get_uint32(const std::string& attr_name) const;

        //! Returns a 16-bit signed integer for the attribute indicated by a given position.
        //int16_t get_int16(const std::size_t attr_pos) const;

        //! Returns a 16-bit signed integer for the attribute indicated by a given name.
        //int16_t get_int16(const std::string& attr_name) const;

        //! Returns a 16-bit unsigned integer for the attribute indicated by a given position.
        //uint16_t get_uint16(const std::size_t attr_pos) const;

        //! Returns a 16-bit unsigned integer for the attribute indicated by a given name.
        //uint16_t get_uint16(const std::string& attr_name) const;

        //! Returns a 8-bit unsigned integer for the attribute indicated by a given position.
        uint8_t get_uint8(const std::size_t attr_pos) const;

        //! Returns a 8-bit unsigned integer for the attribute indicated by a given name.
        uint8_t get_uint8(const std::string& attr_name) const;

        //! Returns a boolean value for the attribute indicated by a given position.
        //bool get_bool(const std::size_t attr_pos) const;

        //! Returns a boolean value for the attribute indicated by a given name.
        //bool get_bool(const std::string& attr_name) const;

        //! Returns the number of cell attributes.
        //std::size_t num_attributes() const;

        //! Returns the attribute name at position pos.
        //const std::string& attribute_name(const std::size_t pos) const;

        //! Returns the attribute position for a named attribute.
        std::size_t attribute_pos(const std::string& name) const;

        //! Move iterator to the next cell.
        void next();

        //! Returns true if iterator has finished traversing the array.
        bool end() const;

        //! Syntatic suggar operator.
        cell_iterator& operator++();

      private:

        class impl;

        impl* pimpl_;

        bool at_end_;
    };

    inline bool cell_iterator::end() const
    {
      return at_end_;
    }
    
  }  // end namespace scidb
}   // end namespace eows

#endif  // __TWS_EOWS_CELL_ITERATOR_HPP__

