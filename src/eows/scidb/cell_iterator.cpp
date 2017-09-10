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
  \file eows/scidb/cell_iterator.cpp

  \brief A cell iterator for a multidimensional array with several attributes.

  \author Gilberto Ribeiro de Queiroz
 */
 
// EOWS
#include "cell_iterator.hpp"

// STL
#include <algorithm>
#include <cassert>
#include <vector>

class eows::scidb::cell_iterator::impl
{
  public:

    impl(const std::shared_ptr< ::scidb::Array >& a, bool& at_end);

    ~impl();

    const ::scidb::Coordinates& get_position();

    //std::string get_str(const std::size_t attr_pos) const;

    //std::string get_str(const std::string& attr_name) const;

//    double get_double(const std::size_t attr_pos) const;

//    double get_double(const std::string& attr_name) const;

      float get_float(const std::size_t attr_pos) const;

      float get_float(const std::string& attr_name) const;

//    int64_t get_int64(const std::size_t attr_pos) const;

//    int64_t get_int64(const std::string& attr_name) const;

//    uint64_t get_uint64(const std::size_t attr_pos) const;

//    uint64_t get_uint64(const std::string& attr_name) const;

    uint32_t get_uint32(const std::size_t attr_pos) const;

    uint32_t get_uint32(const std::string& attr_name) const;

    int32_t get_int32(const std::size_t attr_pos) const;

    int32_t get_int32(const std::string& attr_name) const;

    int16_t get_int16(const std::size_t attr_pos) const;

    int16_t get_int16(const std::string& attr_name) const;

    int8_t get_int8(const std::size_t attr_pos) const;

    int8_t get_int8(const std::string& attr_name) const;

    uint16_t get_uint16(const std::size_t attr_pos) const;

    uint16_t get_uint16(const std::string& attr_name) const;

    uint8_t get_uint8(const std::size_t attr_pos) const;

    uint8_t get_uint8(const std::string& attr_name) const;

//    bool get_bool(const std::size_t attr_pos) const;

//    bool get_bool(const std::string& attr_name) const;

//    std::size_t num_attributes() const;

//    const std::string& attribute_name(const std::size_t pos) const;

    std::size_t attribute_pos(const std::string& name) const;

    void next();

  private:

    std::shared_ptr< ::scidb::Array > array_;
    std::vector< std::shared_ptr< ::scidb::ConstArrayIterator > > attribute_iterators_;
    std::vector< std::shared_ptr< ::scidb::ConstChunkIterator > > chunks_iterators_;
    std::vector<std::string> attr_names_;
    std::size_t num_attributes_;
    bool& at_end_;
};

eows::scidb::cell_iterator::cell_iterator(const std::shared_ptr< ::scidb::Array >& a)
  : pimpl_(nullptr),
    at_end_(false)
{
  pimpl_ = new impl(a, at_end_);
}

eows::scidb::cell_iterator::~cell_iterator()
{
  delete pimpl_;
}

const ::scidb::Coordinates&
eows::scidb::cell_iterator::get_position()
{
  return pimpl_->get_position();
}

//std::string
//eows::scidb::cell_iterator::get_str(const std::size_t attr_pos) const
//{
//  return pimpl_->get_str(attr_pos);
//}

//std::string
//eows::scidb::cell_iterator::get_str(const std::string& attr_name) const
//{
//  return pimpl_->get_str(attr_name);
//}

//double
//eows::scidb::cell_iterator::get_double(const std::size_t attr_pos) const
//{
//  return pimpl_->get_double(attr_pos);
//}

//double
//eows::scidb::cell_iterator::get_double(const std::string& attr_name) const
//{
//  return pimpl_->get_double(attr_name);
//}

float
eows::scidb::cell_iterator::get_float(const std::size_t attr_pos) const
{
  return pimpl_->get_float(attr_pos);
}

float
eows::scidb::cell_iterator::get_float(const std::string& attr_name) const
{
  return pimpl_->get_float(attr_name);
}

//int64_t
//eows::scidb::cell_iterator::get_int64(const std::size_t attr_pos) const
//{
//  return pimpl_->get_int64(attr_pos);
//}

//int64_t
//eows::scidb::cell_iterator::get_int64(const std::string& attr_name) const
//{
//  return pimpl_->get_int64(attr_name);
//}

//uint64_t
//eows::scidb::cell_iterator::get_uint64(const std::size_t attr_pos) const
//{
//  return pimpl_->get_uint64(attr_pos);
//}

//uint64_t
//eows::scidb::cell_iterator::get_uint64(const std::string& attr_name) const
//{
//  return pimpl_->get_uint64(attr_name);
//}

uint32_t
eows::scidb::cell_iterator::get_uint32(const std::size_t attr_pos) const
{
  return pimpl_->get_uint32(attr_pos);
}

uint32_t
eows::scidb::cell_iterator::get_uint32(const std::string& attr_name) const
{
  return pimpl_->get_uint32(attr_name);
}

int32_t
eows::scidb::cell_iterator::get_int32(const std::size_t attr_pos) const
{
  return pimpl_->get_int32(attr_pos);
}

int32_t
eows::scidb::cell_iterator::get_int32(const std::string& attr_name) const
{
  return pimpl_->get_int32(attr_name);
}

int16_t
eows::scidb::cell_iterator::get_int16(const std::size_t attr_pos) const
{
  return pimpl_->get_int16(attr_pos);
}

int16_t
eows::scidb::cell_iterator::get_int16(const std::string& attr_name) const
{
  return pimpl_->get_int16(attr_name);
}

int8_t
eows::scidb::cell_iterator::get_int8(const std::size_t attr_pos) const
{
  return pimpl_->get_int8(attr_pos);
}

int8_t
eows::scidb::cell_iterator::get_int8(const std::string& attr_name) const
{
  return pimpl_->get_int8(attr_name);
}

//uint16_t
//eows::scidb::cell_iterator::get_uint16(const std::size_t attr_pos) const
//{
//  return pimpl_->get_uint16(attr_pos);
//}

uint16_t
eows::scidb::cell_iterator::get_uint16(const std::string& attr_name) const
{
  return pimpl_->get_uint16(attr_name);
}

uint8_t
eows::scidb::cell_iterator::get_uint8(const std::size_t attr_pos) const
{
  return pimpl_->get_uint8(attr_pos);
}

uint8_t
eows::scidb::cell_iterator::get_uint8(const std::string& attr_name) const
{
  return pimpl_->get_uint8(attr_name);
}

//bool
//eows::scidb::cell_iterator::get_bool(const std::size_t attr_pos) const
//{
//  return pimpl_->get_bool(attr_pos);
//}

//bool
//eows::scidb::cell_iterator::get_bool(const std::string& attr_name) const
//{
//  return pimpl_->get_bool(attr_name);
//}

//std::size_t
//eows::scidb::cell_iterator::num_attributes() const
//{
//  return pimpl_->num_attributes();
//}

//const std::string&
//eows::scidb::cell_iterator::attribute_name(const std::size_t pos) const
//{
//  return pimpl_->attribute_name(pos);
//}

std::size_t
eows::scidb::cell_iterator::attribute_pos(const std::string& name) const
{
  return pimpl_->attribute_pos(name);
}

void eows::scidb::cell_iterator::next()
{
  pimpl_->next();
}

eows::scidb::cell_iterator&
eows::scidb::cell_iterator::operator++()
{
  pimpl_->next();
  
  return *this;
}

inline
eows::scidb::cell_iterator::impl::impl(const std::shared_ptr< ::scidb::Array >& a, bool& at_end)
  : array_(a),
    num_attributes_(0),
    at_end_(at_end)
{
  const ::scidb::ArrayDesc& description = array_->getArrayDesc();

  const ::scidb::Attributes& attrs = description.getAttributes(true);

  for(const ::scidb::AttributeDesc& att_description : attrs)
  {
    attr_names_.push_back(att_description.getName());

    std::shared_ptr< ::scidb::ConstArrayIterator > it = array_->getConstIterator(att_description.getId());

    attribute_iterators_.push_back(it);

    if(it->end())
    {
      at_end_ = true;

      chunks_iterators_.push_back(std::shared_ptr< ::scidb::ConstChunkIterator >());
    }
    else
    {
      const ::scidb::ConstChunk& chunk = it->getChunk();

      std::shared_ptr< ::scidb::ConstChunkIterator > cit = chunk.getConstIterator();

      if(cit->end())
        at_end_ = true;

      chunks_iterators_.push_back(cit);
    }
  }

  num_attributes_ = attr_names_.size();
}

inline
eows::scidb::cell_iterator::impl::~impl()
{
}

inline const ::scidb::Coordinates&
eows::scidb::cell_iterator::impl::get_position()
{
  return chunks_iterators_[0]->getPosition();
}

//inline std::string
//eows::scidb::cell_iterator::impl::get_str(const std::size_t attr_pos) const
//{
//  const ::scidb::Value& v = chunks_iterators_[attr_pos]->getItem();

//  return v.getString();
//}

//inline std::string
//eows::scidb::cell_iterator::impl::get_str(const std::string& attr_name) const
//{
//  return get_str(attribute_pos(attr_name));
//}

//inline double
//eows::scidb::cell_iterator::impl::get_double(const std::size_t pos) const
//{
//  const ::scidb::Value& v = chunks_iterators_[pos]->getItem();

//  return v.getDouble();
//}

//inline double
//eows::scidb::cell_iterator::impl::get_double(const std::string& attr_name) const
//{
//  return get_double(attribute_pos(attr_name));
//}

inline float
eows::scidb::cell_iterator::impl::get_float(const std::size_t pos) const
{
  const ::scidb::Value& v = chunks_iterators_[pos]->getItem();

  return v.getFloat();
}

inline float 
eows::scidb::cell_iterator::impl::get_float(const std::string& attr_name) const
{
  return get_float(attribute_pos(attr_name));
}



//inline int64_t
//eows::scidb::cell_iterator::impl::get_int64(const std::size_t pos) const
//{
//  const ::scidb::Value& v = chunks_iterators_[pos]->getItem();

//  return v.getInt64();
//}

//inline int64_t
//eows::scidb::cell_iterator::impl::get_int64(const std::string& attr_name) const
//{
//  return get_int64(attribute_pos(attr_name));
//}

//inline uint64_t
//eows::scidb::cell_iterator::impl::get_uint64(const std::size_t pos) const
//{
//  const ::scidb::Value& v = chunks_iterators_[pos]->getItem();

//  return v.getUint64();
//}

//inline uint64_t
//eows::scidb::cell_iterator::impl::get_uint64(const std::string& attr_name) const
//{
//  return get_uint64(attribute_pos(attr_name));
//}

inline uint32_t
eows::scidb::cell_iterator::impl::get_uint32(const std::size_t pos) const
{
  const ::scidb::Value& v = chunks_iterators_[pos]->getItem();

  return v.getUint32();
}

inline uint32_t
eows::scidb::cell_iterator::impl::get_uint32(const std::string& attr_name) const
{
  return get_uint32(attribute_pos(attr_name));
}

inline int32_t
eows::scidb::cell_iterator::impl::get_int32(const std::size_t pos) const
{
  const ::scidb::Value& v = chunks_iterators_[pos]->getItem();

  return v.getInt32();
}

inline int32_t
eows::scidb::cell_iterator::impl::get_int32(const std::string& attr_name) const
{
  return get_int32(attribute_pos(attr_name));
}

inline int16_t
eows::scidb::cell_iterator::impl::get_int16(const std::size_t pos) const
{
  const ::scidb::Value& v = chunks_iterators_[pos]->getItem();

  return v.getInt16();
}

inline int16_t
eows::scidb::cell_iterator::impl::get_int16(const std::string& attr_name) const
{
  return get_int16(attribute_pos(attr_name));
}

inline int8_t
eows::scidb::cell_iterator::impl::get_int8(const std::size_t pos) const
{
  const ::scidb::Value& v = chunks_iterators_[pos]->getItem();

  return v.getInt8();
}

inline int8_t
eows::scidb::cell_iterator::impl::get_int8(const std::string& attr_name) const
{
  return get_int8(attribute_pos(attr_name));
}

inline uint16_t
eows::scidb::cell_iterator::impl::get_uint16(const std::size_t pos) const
{
  const ::scidb::Value& v = chunks_iterators_[pos]->getItem();
  
  return v.getUint16();
}

inline uint16_t
eows::scidb::cell_iterator::impl::get_uint16(const std::string& attr_name) const
{
  return get_uint16(attribute_pos(attr_name));
}

inline uint8_t
eows::scidb::cell_iterator::impl::get_uint8(const std::size_t pos) const
{
  const ::scidb::Value& v = chunks_iterators_[pos]->getItem();

  return v.getUint8();
}

inline uint8_t
eows::scidb::cell_iterator::impl::get_uint8(const std::string& attr_name) const
{
  return get_uint8(attribute_pos(attr_name));
}

//inline bool
//eows::scidb::cell_iterator::impl::get_bool(const std::size_t pos) const
//{
//  const ::scidb::Value& v = chunks_iterators_[pos]->getItem();

//  return v.getBool();
//}

//inline bool
//eows::scidb::cell_iterator::impl::get_bool(const std::string& attr_name) const
//{
//  return get_bool(attribute_pos(attr_name));
//}

//inline std::size_t
//eows::scidb::cell_iterator::impl::num_attributes() const
//{
//  return attr_names_.size();
//}

//inline const std::string&
//eows::scidb::cell_iterator::impl::attribute_name(const std::size_t pos) const
//{
//  return attr_names_[pos];
//}

inline std::size_t
eows::scidb::cell_iterator::impl::attribute_pos(const std::string& name) const
{
  return std::find(attr_names_.begin(), attr_names_.end(), name) - attr_names_.begin();
}

inline void
eows::scidb::cell_iterator::impl::next()
{
  for(std::size_t i = 0; i != num_attributes_; ++i)
  {
    ++(*chunks_iterators_[i]);

    if(chunks_iterators_[i]->end())
    {
// is there more chunks?
      ++(*attribute_iterators_[i]);

      if(attribute_iterators_[i]->end())
      {
        at_end_ = true;
      }
      else
      {
        chunks_iterators_[i] = attribute_iterators_[i]->getChunk().getConstIterator();

        if(chunks_iterators_[i]->end())
          at_end_ = true;
      }
    }
  }
}

