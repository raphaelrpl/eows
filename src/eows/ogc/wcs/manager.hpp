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
  \file eows/ogc/wcs/manager.hpp

  \brief This class defines a WCS meta information access

  \author Raphael Willian da Costa
 */

#ifndef __EOWS_OGC_WCS_MANAGER_HPP__
#define __EOWS_OGC_WCS_MANAGER_HPP__

#include <string>

// Boost dependency
#include <boost/noncopyable.hpp>

namespace eows
{
  namespace ogc
  {
    namespace wcs
    {
      namespace core
      {
        // Forward declarations
        struct capabilities_t;
        struct coverage_description_t;
      }

      class manager : private boost::noncopyable
      {
        public:
          ~manager();

          /**
           * @brief It retrieves cached GetCapabilities meta information
           * @return Capabilities instance with values
           */
          const core::capabilities_t& capabilities() const;

          const core::coverage_description_t& describe(const std::string& id) const;
          /**
           * @brief It initializes Manager scope, loading WCS meta information into memory
           */
          void initialize();

          //! Singleton style
          static manager& instance();

        private:
          //! Singleton style
          manager();
          //! Pimpl idiom
          struct impl;
          impl* pimpl_;
      };
    }
  }
}

#endif //__EOWS_OGC_WCS_MANAGER_HPP__
