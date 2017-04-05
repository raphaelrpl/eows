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
  \file eows/ogc/wcs/core/logger.hpp

  \brief Represents WCS Logger module

  \author Raphael Willian da Costa
 */

#ifndef __EOWS_OGC_WCS_CORE_LOGGER_HPP__
#define __EOWS_OGC_WCS_CORE_LOGGER_HPP__

#include "../../../core/logger.hpp"

#define EOWS_WCS_LOG_TRACE(message) \
        BOOST_LOG_CHANNEL_SEV(eows::core::m_logger, \
                              "wcs", \
                              boost::log::trivial::trace) << message

#define EOWS_WCS_LOG_DEBUG(message) \
        BOOST_LOG_CHANNEL_SEV(eows::core::m_logger, \
                              "wcs", \
                              boost::log::trivial::debug) << message

#define EOWS_WCS_LOG_INFO(message) \
        BOOST_LOG_CHANNEL_SEV(eows::core::m_logger, \
                              "wcs", \
                              boost::log::trivial::info) << message

#define EOWS_WCS_LOG_WARN(message) \
        BOOST_LOG_CHANNEL_SEV(eows::core::m_logger, \
                              "wcs", \
                              boost::log::trivial::warning) << message

#define EOWS_WCS_LOG_ERROR(message) \
        BOOST_LOG_CHANNEL_SEV(eows::core::m_logger, \
                              "wcs", \
                              boost::log::trivial::error) << message

#define EOWS_WCS_LOG_FATAL(message) \
        BOOST_LOG_CHANNEL_SEV(eows::core::m_logger, \
                              "wcs", \
                              boost::log::trivial::fatal) << message

#endif //__EOWS_OGC_WCS_CORE_LOGGER_HPP__
