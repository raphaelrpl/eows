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
  \file eows/core/file_remover.hpp

  \brief Class for file remover.

  \author Raphael Willian da Costa
 */

#ifndef __EOWS_CORE_FILE_REMOVER_HPP__
#define __EOWS_CORE_FILE_REMOVER_HPP__

// STL
#include <set>
#include <memory>

// Boost
#include <boost/noncopyable.hpp>

namespace eows
{
  namespace core
  {
    /*!
     * \brief This class defines a auto file remover once out scope. It is useful to remove temporary files
     *        Make sure to keep object live til required to delete.
     */
    class file_remover : private boost::noncopyable
    {
      public:
        file_remover() = default;
        virtual ~file_remover();

        // rvalue
        file_remover(file_remover&& rhs) = default;

        /*!
         * \brief It appends a file name ref to stack
         * \param file - File name
         */
        void add(const std::string& file);

      private:
        std::set<std::string> files_; //!< Stored files
    };

    typedef std::shared_ptr<file_remover> file_remover_ptr;
  }
}

#endif // __EOWS_CORE_FILE_REMOVER_HPP__
