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
  \file eows/proj4/converter.hpp

  \brief Projection Convertion Module handler

  \author Raphael Willian da Costa
 */

#ifndef __EOWS_PROJ4_CONVERTER_HPP__
#define __EOWS_PROJ4_CONVERTER_HPP__

#include <boost/noncopyable.hpp>

namespace eows
{
  namespace proj4
  {
    /*!
     * \brief Class for projection converting
     *
     * In order to use it, pass both valid source srid and target srid and then perform convert operation to retrieve cast values.
     *
     * \note Works only EOWS supported SRID
     */
    class converter : private boost::noncopyable
    {
      public:
        //! Constructor. TODO: Pass source and target srid also into constructor
        converter();

        //! Destructor
        ~converter();

        /*!
         * \brief Sets converter source SRID
         * \throws std::exception When SRID not supported
         * \param srid - SRID value
         */
        void set_source_srid(const int& srid);

        /*!
         * \brief Sets converter targete SRID
         * \throws std::exception When SRID not supported
         * \param srid - SRID value
         */
        void set_target_srid(const int& srid);

        /*!
         * \brief Performs a projection convertion from source srid to target srid.
         *
         * \throws std::runtime_error when could not convert coordinate
         * \param x - Value of X to reproject
         * \param y - Value of Y to reproject
         */
        void convert(double& x, double& y) const;
      private:
        struct impl;
        impl* pimpl_;
    };
  }
}

#endif //__EOWS_PROJ4_CONVERTER_HPP__
