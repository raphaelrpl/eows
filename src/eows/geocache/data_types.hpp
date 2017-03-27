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
  \file eows/geocache/data_types.hpp

  \brief Some usefull data types for EOWS.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_GEOCACHE_DATA_TYPES_HPP__
#define __EOWS_GEOCACHE_DATA_TYPES_HPP__

namespace eows
{
  namespace geocache
  {

    //! Cache information about a dataset.
    struct cache_info
    {
      uint64_t mem_block_cache_size;   //!< Maximum size in bytes for a block cache in RAM.
      uint64_t file_block_cache_size;  //!< Maximum size in bytes for a block cache in the File System.
      std::map<std::string, cache_info> dataset_cache;  //!< Specific cache size for each dataset.
    };

      std::size_t num_time_points_;         //!< Number of bytes occupied for all pixel values.
      std::size_t pixel_value_size_;        //!< Number of bytes occupied for a single value of the pixel.
      std::size_t pixel_size_;              //!< Number of bytes occupied for all pixel values.
      std::size_t data_block_size_;         //!< Number of bytes occupied for all pixels in a block in time.
      std::size_t pixel_offset_size_;       //!< Number of bytes we need to skip in a block to get the next value of a pixel.
      std::size_t num_blocks_per_tile_;     //!< Number of data blocks in a single tile.
      std::size_t total_blocks_in_x_;       //!< Number of data blocks among all tiles in a x direction.
    
  } // end namespace core
}   // end namespace eows

#endif // __EOWS_GEOCACHE_DATA_TYPES_HPP__

