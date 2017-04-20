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
  \file eows/geoarray/utils.hpp

  \brief General utility functions.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_GEOARRAY_UTILS_HPP__
#define __EOWS_GEOARRAY_UTILS_HPP__

// EOWS
#include "../core/utils.hpp"
#include "../proj4/srs.hpp"
#include "data_types.hpp"

// RapidJSON
#include <rapidjson/document.h>
#include <rapidjson/writer.h>

namespace eows
{
  namespace geoarray
  {
    /*!
      \exception eows::parse_error If it is detected a missing key or ill formed item.
     */
    geoarray_t
    read_geo_array(const rapidjson::Value& jgeo_array);
    
    /*!
      \exception eows::parse_error If it is detected a missing key or ill formed item.
     */
    std::vector<attribute_t>
    read_attributes(const rapidjson::Value& jattributes);
    
    /*!
      \exception eows::parse_error If it is detected a missing key or ill formed item.
     */
    attribute_t
    read_attribute(const rapidjson::Value& jattribute);
    
    /*!
      \exception eows::parse_error If it is detected a missing key or ill formed item.
     */
    dimensions_t
    read_dimensions(const rapidjson::Value& jdimensions);
    
    /*!
      \exception eows::parse_error If it is detected a missing key or ill formed item.
     */
    dimension_t
    read_dimension(const rapidjson::Value& jdimension);

    /*!
      \exception eows::parse_error If it is detected a missing key or ill formed item.
     */
    spatial_extent_t
    read_spatial_extent(const rapidjson::Value& jspatial_extent);
    
    /*!
      \exception eows::parse_error If it is detected a missing key or ill formed item.
     */
    numeric_range_t
    read_valid_range(const rapidjson::Value& jvalid_range);
    
    /*!
      \exception eows::parse_error If it is detected a missing key or ill formed item.
     */
    timeline_t
    read_timeline(const rapidjson::Value& jtimeline, const dimension_t& t);

    /*!
      \exception eows::parse_error If it is detected a missing key or ill formed item.
     */
    spatial_resolution_t
    read_resolution(const rapidjson::Value& jresolution);

    /*!
      \exception eows::parse_error If it is detected a missing key or ill formed item.
     */
    internal_metadata_t
    read_internal_metadata(const rapidjson::Value& jinternal_metadata);

    /*!
     * \brief It retrieves a geoarray subset by name
     * \return Dimension iterator
     */
    std::vector<eows::geoarray::dimension_t>::const_iterator find_by_name(const std::vector<eows::geoarray::dimension_t>&,
                                                                          const std::string&);

    template<class Writer>
    void write(Writer& writer, const dimension_t& dim);

    template<class Writer>
    void write(Writer& writer, const dimensions_t& dims);

    template<class Writer>
    void write(Writer& writer, const numeric_range_t& range);

    template<class Writer>
    void write(Writer& writer, const attribute_t& attr);

    template<class Writer>
    void write(Writer& writer, const spatial_resolution_t& res);

    template<class Writer>
    void write(Writer& writer, const spatial_extent_t& extent);

    template<class Writer>
    void write(Writer& writer, const geoarray_t& geo_array);

    /*!
      \exception std::exception May throw exceptions during initialization.
     */
    void initialize();

  }  // end namespace geoarray
}    // end namespace eows

template<class Writer>
void eows::geoarray::write(Writer& writer, const dimension_t& dim)
{
  writer.StartObject();

  writer.Key("name", static_cast<rapidjson::SizeType>(sizeof("name") -1));
  writer.String(dim.name.c_str(), static_cast<rapidjson::SizeType>(dim.name.length()));

  writer.Key("min_idx", static_cast<rapidjson::SizeType>(sizeof("min_idx") -1));
  writer.Uint(dim.min_idx);

  writer.Key("max_idx", static_cast<rapidjson::SizeType>(sizeof("max_idx") -1));
  writer.Uint(dim.max_idx);

  writer.EndObject();
}

template<class Writer>
void eows::geoarray::write(Writer& writer, const dimensions_t& dims)
{
  writer.StartObject();

  writer.Key("x", static_cast<rapidjson::SizeType>(sizeof("x") -1));
  write(writer, dims.x);

  writer.Key("y", static_cast<rapidjson::SizeType>(sizeof("y") -1));
  write(writer, dims.y);

  writer.Key("t", static_cast<rapidjson::SizeType>(sizeof("t") -1));
  write(writer, dims.t);

  writer.EndObject();
}

template<class Writer>
void eows::geoarray::write(Writer& writer, const numeric_range_t& range)
{
  writer.StartObject();

  writer.Key("min", static_cast<rapidjson::SizeType>(sizeof("min") -1));
  writer.Double(range.min_val);

  writer.Key("max", static_cast<rapidjson::SizeType>(sizeof("max") -1));
  writer.Double(range.max_val);

  writer.EndObject();
}

template<class Writer>
void eows::geoarray::write(Writer& writer, const attribute_t& attr)
{
  writer.StartObject();

  writer.Key("name", static_cast<rapidjson::SizeType>(sizeof("name") -1));
  writer.String(attr.name.c_str(), static_cast<rapidjson::SizeType>(attr.name.length()));

  writer.Key("description", static_cast<rapidjson::SizeType>(sizeof("description") -1));
  writer.String(attr.description.c_str(), static_cast<rapidjson::SizeType>(attr.description.length()));

  writer.Key("datatype", static_cast<rapidjson::SizeType>(sizeof("datatype") -1));
  std::string sdt = eows::geoarray::datatype_t::to_string(attr.datatype);
  writer.String(sdt.c_str(), static_cast<rapidjson::SizeType>(sdt.length()));

  writer.Key("valid_range", static_cast<rapidjson::SizeType>(sizeof("valid_range") - 1));
  write(writer, attr.valid_range);

  writer.Key("scale_factor", static_cast<rapidjson::SizeType>(sizeof("scale_factor") -1));
  writer.Double(attr.scale_factor);

  writer.Key("missing_value", static_cast<rapidjson::SizeType>(sizeof("missing_value") -1));
  writer.Double(attr.missing_value);

  writer.EndObject();
}

template<class Writer>
void eows::geoarray::write(Writer& writer, const spatial_resolution_t& res)
{
  writer.StartObject();

  writer.Key("x", static_cast<rapidjson::SizeType>(sizeof("x") - 1));
  writer.Double(res.x);

  writer.Key("y", static_cast<rapidjson::SizeType>(sizeof("y") - 1));
  writer.Double(res.y);

  writer.EndObject();
}

template<class Writer>
void eows::geoarray::write(Writer& writer, const spatial_extent_t& extent)
{
  writer.StartObject();

  writer.Key("xmin", static_cast<rapidjson::SizeType>(sizeof("xmin") - 1));
  writer.Double(extent.xmin);

  writer.Key("ymin", static_cast<rapidjson::SizeType>(sizeof("ymin") - 1));
  writer.Double(extent.ymin);

  writer.Key("xmax", static_cast<rapidjson::SizeType>(sizeof("xmax") - 1));
  writer.Double(extent.xmax);

  writer.Key("ymax", static_cast<rapidjson::SizeType>(sizeof("ymax") - 1));
  writer.Double(extent.ymax);

  writer.EndObject();
}

template<class Writer>
void eows::geoarray::write(Writer& writer, const geoarray_t& geo_array)
{
  writer.StartObject();

  writer.Key("name", static_cast<rapidjson::SizeType>(sizeof("name") -1));
  writer.String(geo_array.name.c_str(), static_cast<rapidjson::SizeType>(geo_array.name.length()));

  writer.Key("description", static_cast<rapidjson::SizeType>(sizeof("description") - 1));
  writer.String(geo_array.description.c_str(), static_cast<rapidjson::SizeType>(geo_array.description.length()));

  writer.Key("detail", static_cast<rapidjson::SizeType>(sizeof("detail") - 1));
  writer.String(geo_array.detail.c_str(), static_cast<rapidjson::SizeType>(geo_array.detail.length()));

  writer.Key("dimensions", static_cast<rapidjson::SizeType>(sizeof("dimensions") - 1));
  write(writer, geo_array.dimensions);

  writer.Key("attributes", static_cast<rapidjson::SizeType>(sizeof("attributes") - 1));
  writer.StartArray();
  std::for_each(geo_array.attributes.begin(),
                geo_array.attributes.end(),
                [&writer](const attribute_t& att) -> void { write(writer, att); });
  writer.EndArray();

  writer.Key("spatial_extent", static_cast<rapidjson::SizeType>(sizeof("spatial_extent") - 1));
  write(writer, geo_array.spatial_extent);

  writer.Key("spatial_resolution", static_cast<rapidjson::SizeType>(sizeof("spatial_resolution") - 1));
  write(writer, geo_array.spatial_resolution);

  writer.Key("crs", static_cast<rapidjson::SizeType>(sizeof("crs") - 1));

  writer.StartObject();

  const eows::proj4::srs_description_t& crs = eows::proj4::srs_manager::instance().get(geo_array.srid);

  writer.Key("proj4", static_cast<rapidjson::SizeType>(sizeof("proj4") - 1));
  writer.String(crs.proj4_txt.c_str(), static_cast<rapidjson::SizeType>(crs.proj4_txt.length()));

  writer.Key("wkt", static_cast<rapidjson::SizeType>(sizeof("wkt") - 1));
  writer.String(crs.wkt.c_str(), static_cast<rapidjson::SizeType>(crs.wkt.length()));

  writer.EndObject();

  writer.Key("timeline", static_cast<rapidjson::SizeType>(sizeof("timeline") - 1));
  eows::core::write_string_array(geo_array.timeline.time_points().begin(),
                                 geo_array.timeline.time_points().end(),
                                 writer);

  writer.EndObject();
}

#endif  // __EOWS_GEOARRAY_UTILS_HPP__
