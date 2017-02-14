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
  \file eows/wtss/metadata_manager.hpp

  \brief A singleton for managin metadata for WTSS operations.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_WTSS_METADATA_MANAGER_HPP__
#define __EOWS_WTSS_METADATA_MANAGER_HPP__

// STL
#include <string>

// Boost
#include <boost/noncopyable.hpp>

namespace eows
{
  namespace wtss
  {

    class metadata_manager : public boost::noncopyable
    {
      public:

        const std::string& get_coverage_list() const;

        /*!
         * \brief get_coverage_metadata
         * \param name
         * \return
         * \exception std::out_of_range If a coverage with the given name doesn't exist;
         */
        const std::string& get_coverage_metadata(const std::string& name) const;

        //! Access the singleton.
        static metadata_manager& instance();

      private:

        //! Constructor.
        metadata_manager() = default;

        //! Destructor.
        ~metadata_manager() = default;

      private:

        struct impl;

        impl* pimpl_;
    };

  }   // end namespace wtss
}     // end namespace eows

#endif  // __EOWS_WTSS_METADATA_MANAGER_HPP__

