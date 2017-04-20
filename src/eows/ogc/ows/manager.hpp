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
  \file eows/ogc/ows/manager.hpp

  \brief This class defines a OWS meta information access

  \author Raphael Willian da Costa
 */

#ifndef __EOWS_OGC_OWS_MANAGER_HPP__
#define __EOWS_OGC_OWS_MANAGER_HPP__

// Boost dependency
#include <boost/noncopyable.hpp>

namespace eows
{
  namespace ogc
  {
    namespace ows
    {
      // Forward declarations
      struct provider_t;

      class manager : private boost::noncopyable
      {
        public:
          ~manager();

          /*!
           * \brief It retrieves cached OWS provider meta information
           * \returns cached Provider object
           */
          const provider_t& provider() const;
          /**
           * @brief It initializes Manager scope, loading OWS meta information into memory
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

#endif //__EOWS_OGC_OWS_MANAGER_HPP__
