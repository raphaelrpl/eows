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
  \file eows/gdal/dataset.hpp

  \brief Defines for GDAL DataSet.

  \author Raphael Willian da Costa
 */

#ifndef __EOWS_CORE_DATASET_HPP__
#define __EOWS_CORE_DATASET_HPP__

// STL
#include <memory>
// Boost noncopyable
#include <boost/noncopyable.hpp>

namespace eows
{
  namespace core
  {
    class dataset : private boost::noncopyable
    {
      public:
        enum type_t
        {
          INT8,
          UINT8,
          INT16,
          UINT16,
          INT32,
          UINT32,
          INT64
        };

        dataset();
        virtual ~dataset();

        template<typename DataType>
        DataType get(const std::string& name, type_t t)
        {
          switch(t)
          {
            case INT8:
              return get_int8(name);
              break;
            case UINT8:
              break;
            case INT16:
              break;
            case UINT16:
              break;
            case INT32:
              break;
            case UINT32:
              break;
            case INT64:
              break;
            default:
              throw std::runtime_error("Invalid type_t");
          }
        }

        virtual int8_t get_int8(const std::string& name) = 0;

        virtual uint8_t get_uint8(const std::string& name) = 0;

        virtual int16_t get_int16(const std::string& name) = 0;

        virtual uint16_t get_uint16(const std::string& name) = 0;

      private:
        struct impl;
    };

    typedef std::shared_ptr<dataset> dataset_ptr;
  }
}

#endif // __EOWS_CORE_DATASET_HPP__
