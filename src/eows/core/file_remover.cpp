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
  \file eows/core/file_remover.cpp

  \brief Implementation of file remover.

  \author Raphael Willian da Costa
 */

// Definition
#include "file_remover.hpp"

// EOWS Logger
#include "logger.hpp"

// Boost (Remove File)
#include <boost/filesystem/operations.hpp>

eows::core::file_remover::~file_remover()
{
  for(auto& file: files_)
  {
    try
    {
      boost::filesystem::remove(file);
      std::cout << "Removing " << file << std::endl;
    } catch (...)
    {
      EOWS_LOG_DEBUG("Could not remove " + file + ". Maybe it's already been deleted by OS");
    }
  }
}

void eows::core::file_remover::add(const std::string &file)
{
  files_.emplace(file);
}
