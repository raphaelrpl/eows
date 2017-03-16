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
  \file eows/ogc/wms/manager.hpp

  \brief A singleton for managing the WMS capabilities.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_OGC_WMS_MANAGER_HPP__
#define __EOWS_OGC_WMS_MANAGER_HPP__

// Boost
#include <boost/noncopyable.hpp>

namespace eows
{
  namespace ogc
  {
    namespace wms
    {

      struct capabilities_t;

      class manager : public boost::noncopyable
      {
        public:

          const capabilities_t& get_capabilities() const;

          void initialize();

          static manager& instance();

        protected:

          manager();

          ~manager();

        private:

          struct impl;

          impl* pimpl_;
      };

    }  // end namespace wms
  }    // end namespace ogc
}      // end namespace eows

#endif // __EOWS_OGC_WMS_MANAGER_HPP__
