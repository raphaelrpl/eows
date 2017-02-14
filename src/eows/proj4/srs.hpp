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
  \file eows/proj4/srs.hpp

  \brief A simple wrapper for Proj.4 support.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_PROJ4_SRS_HPP__
#define __EOWS_PROJ4_SRS_HPP__

// STL
#include <map>
#include <string>

// Boost
#include <boost/format.hpp>
#include <boost/noncopyable.hpp>

// Proj.4
#include <proj_api.h>

namespace eows
{
  namespace proj4
  {

    struct spatial_reference
    {
      void* pj_;

      spatial_reference(const std::string& proj4_txt)
        : pj_(nullptr)
      {
        pj_ = pj_init_plus(proj4_txt.c_str());

        if(pj_ == nullptr)
        {
          boost::format err_msg("Could not initialize projection: '%1%'.");
          throw std::runtime_error((err_msg % proj4_txt).str());
        }
      }

      ~spatial_reference()
      {
        pj_free(pj_);
      }
    };

    inline void
    transform(spatial_reference& src, spatial_reference& dst, double& x, double& y)
    {
      if(pj_is_latlong(src.pj_))
      {
        x = x * DEG_TO_RAD;
        y = y * DEG_TO_RAD;
      }

      int retval = pj_transform(src.pj_, dst.pj_, 1, 0, &x, &y, 0);

      if(retval != 0)
        throw std::runtime_error("Coordinate transformation error.");

      if(pj_is_latlong(dst.pj_))
      {
        x = x * RAD_TO_DEG;
        y = y * RAD_TO_DEG;
      }
    }

    typedef std::map<std::size_t, std::unique_ptr<spatial_reference> > spatial_ref_map;

    struct srs_description_t
    {
      std::string proj4_txt;
      std::string wkt;
    };

    class srs_manager: public boost::noncopyable
    {
      public:

        void insert(std::size_t srid, const srs_description_t& srs);

        const srs_description_t& get(std::size_t srid) const;

        bool exists(std::size_t srid) const;

        //! Access the singleton.
        static srs_manager& instance();

      protected:

        srs_manager();

        ~srs_manager();

      private:

        struct impl;
        impl* pimpl_;
    };

  } //end namespace proj4
}   // end namespace eows

#endif // __EOWS_PROJ4_SRS_HPP__
