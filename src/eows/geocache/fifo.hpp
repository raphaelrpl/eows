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
  \file eows/geocache/utils.hpp

  \brief A class for representing a data cache based on FIFO strategy.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_GEOCACHE_FIFO_HPP__
#define __EOWS_GEOCACHE_FIFO_HPP__

// EOWS
#include "exception.hpp"

// STL
#include <cstdint>
#include <map>
#include <vector>


// Boost
#include <boost/noncopyable.hpp>
#include <boost/shared_array.hpp>

namespace eows
{
  namespace geocache
  {

    //! A class for representing a data cache based on FIFO strategy.
    class fifo : public boost::noncopyable
    {
      public:

        typedef boost::shared_array<unsigned char> cache_value_t;

        fifo();

        ~fifo();

        bool is_full() const;

        cache_value_t data(const uint64_t k) const;

        cache_value_t pop();

        void push(const uint64_t k, const cache_value_t& v);

        void init(const std::size_t max_elements);

        void clear();

      private:

        typedef std::map<uint64_t, cache_value_t> key_value_idx_t;

        key_value_idx_t values_idx_;
        std::vector<uint64_t> fifo_;
        std::size_t fifo_pos_;
        std::size_t max_elements_;
        std::size_t fifo_size_;
    };

  }  // end namespace geocache
}    // end namespace eows

inline eows::geocache::fifo::fifo()
  : fifo_pos_(0),
    max_elements_(0),
    fifo_size_(0)
{
}

inline eows::geocache::fifo::~fifo()
{
}

inline bool eows::geocache::fifo::is_full() const
{
  return fifo_size_ == max_elements_;
}

inline eows::geocache::fifo::cache_value_t
eows::geocache::fifo::data(const uint64_t k) const
{
  key_value_idx_t::const_iterator it = values_idx_.find(k);

  return (it != values_idx_.end()) ? it->second : cache_value_t();
}

inline eows::geocache::fifo::cache_value_t mds::fifo::pop()
{
  assert(fifo_size_ != 0);

  assert(values_idx_.find(fifo_[fifo_pos_]) != values_idx_.end());

  key_value_idx_t::iterator it = values_idx_.find(fifo_[fifo_pos_]);

  if(it == values_idx_.end())
    throw item_not_found_error() << error_description("could not pop data element from cache!");

  --fifo_size_;

  cache_value_t data = it->second;

  values_idx_.erase(it);

  return data;
}

inline void eows::geocache::fifo::push(const uint64_t k, const cache_value_t& v)
{
  assert(!is_full());

  assert(values_idx_.find(k) == values_idx_.end());

  values_idx_[k] = v;

  ++fifo_size_;

  fifo_[fifo_pos_] = k;

  fifo_pos_ = (fifo_pos_ + 1) % max_elements_;
}

inline void eows::geocache::fifo::init(const std::size_t max_elements)
{
  clear();
  
  max_elements_ = max_elements;

  fifo_.resize(max_elements, 0);
}

inline void eows::geocache::fifo::clear()
{
  values_idx_.clear();
  fifo_.clear();
  fifo_pos_ = 0;
  max_elements_ = 0;
  fifo_size_ = 0;
}

#endif // __EOWS_GEOCACHE_FIFO_HPP__

