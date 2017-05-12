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
  \file eows/gdal/dataset_geotiff.hpp

  \brief This class represents a abstraction for GeoTiff Datasets.

  \author Raphael Willian da Costa
 */

#ifndef __EOWS_GDAL_DATASETGEOTIFF_HPP__
#define __EOWS_GDAL_DATASETGEOTIFF_HPP__

#include "exception.hpp"

// STL
#include <string>
#include <memory>
#include <vector>

// GDAL
#include <gdal_priv.h>
#include <cpl_config.h>

// Boost
#include <boost/noncopyable.hpp>

namespace eows
{
  namespace gdal
  {
    /*!
     * \brief Simple implementation of GDAL GeoTiff dataset.
     * \todo Define a Generic dataset and dataset implementations in order to build as factory.
     */
    class dataset_geotiff : private boost::noncopyable
    {
      public:
        dataset_geotiff(const std::string& filename, std::size_t col, std::size_t row, std::size_t bands = 1);
        ~dataset_geotiff();

        /*!
         * \brief It tries to open a dataset.
         * \throw eows::gdal::gdal_error When dataset cannot be opened
         */
        void open();
        /*!
         * \brief It tries to close a dataset. It does not throw exception.
         */
        void close();

        /*!
         * \brief It writes values to grid dataset.
         * \throws eows::gdal::gdal_error When type is invalid or could not write in dataset
         * \param values Data to write
         * \param band Band identifier
         * \param type Data type value. See more in eows::geoarray::datatype_t
         */
        void write_int16(std::vector<GInt16> values, const std::size_t& band);
        void write_int32(std::vector<GInt32> values, const std::size_t& band);
      private:
        std::string format_;
        std::string filename_;
        std::size_t col_;
        std::size_t row_;
        std::size_t bands_;
        GDALDriver* driver_;
        GDALDataset* dset_;
    };

    //! Shared dataset pointer
    typedef std::shared_ptr<dataset_geotiff> dataset_geotiff_ptr;
  }
}

#endif // __EOWS_GDAL_DATASETGEOTIFF_HPP__
