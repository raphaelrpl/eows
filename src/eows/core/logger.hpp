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
  \file eows/core/app_settings.hpp

  \brief A singleton that stores the application settings.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_CORE_LOGGER_HPP__
#define __EOWS_CORE_LOGGER_HPP__

// Boost
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>

namespace eows
{
  namespace core
  {
    static boost::log::sources::severity_channel_logger_mt<boost::log::trivial::severity_level, std::string>  m_logger;
  }
}

#define EOWS_CURRENT_FUNCTION std::string(__PRETTY_FUNCTION__)

#define EOWS_LOG_TRACE(message) \
        BOOST_LOG_CHANNEL_SEV(eows::core::m_logger, \
                              "eows", \
                              boost::log::trivial::trace) << message

#define EOWS_LOG_DEBUG(message) \
        BOOST_LOG_CHANNEL_SEV(eows::core::m_logger, \
                              "eows", \
                              boost::log::trivial::debug) << message

#define EOWS_LOG_INFO(message) \
        BOOST_LOG_CHANNEL_SEV(eows::core::m_logger, \
                              "eows", \
                              boost::log::trivial::info) << message

#define EOWS_LOG_WARN(message) \
        BOOST_LOG_CHANNEL_SEV(eows::core::m_logger, \
                              "eows", \
                              boost::log::trivial::warning) << message

#define EOWS_LOG_ERROR(message) \
        BOOST_LOG_CHANNEL_SEV(eows::core::m_logger, \
                              "eows", \
                              boost::log::trivial::error) << message

#define EOWS_LOG_FATAL(message) \
        BOOST_LOG_CHANNEL_SEV(eows::core::m_logger, \
                              "eows", \
                              boost::log::trivial::fatal) << message

#endif // __EOWS_CORE_LOGGER_HPP__
