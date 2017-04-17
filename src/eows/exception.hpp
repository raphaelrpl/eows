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
  \file eows/exception.hpp

  \brief Base exception class exceptions in EOWS.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_EXCEPTION_HPP__
#define __EOWS_EXCEPTION_HPP__

// STL
#include <stdexcept>
#include <string>

//! Earth Observation Services namespace.
namespace eows
{

  //! An exception indicating an error during the parse os a string or JSON document.
  struct parse_error: virtual std::runtime_error
  {
    explicit parse_error(const std::string& s) : std::runtime_error(s) {}
    
    explicit parse_error(const char* s) : std::runtime_error(s) {}
  };

  struct eows_error : public std::runtime_error
  {
    explicit eows_error(const std::string& s) : std::runtime_error(s) {}
    explicit eows_error(const char* s) : std::runtime_error(s) {}
  };

}   // end namespace eows

#endif // __EOWS_EXCEPTION_HPP__
