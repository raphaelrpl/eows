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
  \file eows/geoarray/utils.cpp

  \brief General utility functions.

  \author Gilberto Ribeiro de Queiroz
 */

// EOWS
#include "utils.hpp"
#include "defines.hpp"
#include "../core/app_settings.hpp"
#include "../core/logger.hpp"
#include "../core/utils.hpp"
#include "exception.hpp"
#include "geoarray_manager.hpp"

// Boost
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

eows::geoarray::geoarray_t
eows::geoarray::read_geo_array(const rapidjson::Value& jgeo_array)
{
  geoarray_t geo_array;

  geo_array.name = eows::core::read_node_as_string(jgeo_array, "name");

  EOWS_LOG_INFO("Preparing metadata about GeoArray: '" + geo_array.name + "'...");

  geo_array.cluster_id = eows::core::read_node_as_string(jgeo_array, "cluster_id");
  geo_array.description = eows::core::read_node_as_string(jgeo_array, "description");
  geo_array.detail = eows::core::read_node_as_string(jgeo_array, "detail");

  rapidjson::Value::ConstMemberIterator jit = jgeo_array.FindMember("dimensions");

  if(jit == jgeo_array.MemberEnd())
    throw eows::parse_error("File '" EOWS_GEOARRAYS_FILE "' is not valid.");

  geo_array.dimensions = read_dimensions(jit->value);

  jit = jgeo_array.FindMember("attributes");

  if(jit == jgeo_array.MemberEnd())
    throw eows::parse_error("File '" EOWS_GEOARRAYS_FILE "' is not valid.");

  geo_array.attributes = read_attributes(jit->value);

  jit = jgeo_array.FindMember("spatial_extent");

  if(jit == jgeo_array.MemberEnd())
    throw eows::parse_error("File '" EOWS_GEOARRAYS_FILE "' is not valid.");

  geo_array.spatial_extent = read_spatial_extent(jit->value);

  jit = jgeo_array.FindMember("spatial_resolution");

  if(jit == jgeo_array.MemberEnd())
    throw eows::parse_error("File '" EOWS_GEOARRAYS_FILE "' is not valid.");

  geo_array.spatial_resolution = read_resolution(jit->value);

  jit = jgeo_array.FindMember("srid");

  if((jit == jgeo_array.MemberEnd()) || (!jit->value.IsUint()))
    throw eows::parse_error("File '" EOWS_GEOARRAYS_FILE "' is not valid.");

  geo_array.srid = jit->value.GetUint();

  jit = jgeo_array.FindMember("timeline");

  if(jit == jgeo_array.MemberEnd())
    throw eows::parse_error("File '" EOWS_GEOARRAYS_FILE "' is not valid.");

  geo_array.timeline = read_timeline(jit->value, geo_array.dimensions.t);

  jit = jgeo_array.FindMember("internal_metadata");

  if(jit == jgeo_array.MemberEnd())
    throw eows::parse_error("File '" EOWS_GEOARRAYS_FILE "' is not valid.");

  geo_array.i_meta = read_internal_metadata(jit->value);

  EOWS_LOG_INFO("Metadata about GeoArray: '" + geo_array.name + "' prepared!");

  return geo_array;
}

std::vector<eows::geoarray::attribute_t>
eows::geoarray::read_attributes(const rapidjson::Value& jattributes)
{
  if(!jattributes.IsArray())
    throw eows::parse_error("File '" EOWS_GEOARRAYS_FILE "' is not valid.");

  std::vector<attribute_t> attributes;
  
  for(rapidjson::SizeType i = 0; i < jattributes.Size(); ++i)
  {
    attribute_t attr = read_attribute(jattributes[i]);
    
    attributes.push_back(attr);
  }
  
  return attributes;
}

eows::geoarray::attribute_t
eows::geoarray::read_attribute(const rapidjson::Value& jattribute)
{
  if(!jattribute.IsObject())
    throw eows::parse_error("File '" EOWS_GEOARRAYS_FILE "' is not valid.");
  
  attribute_t attr;

  attr.name = eows::core::read_node_as_string(jattribute, "name");
  attr.description = eows::core::read_node_as_string(jattribute, "description");
  attr.datatype = datatype_t::from_string(eows::core::read_node_as_string(jattribute, "datatype"));
  
  rapidjson::Value::ConstMemberIterator jit = jattribute.FindMember("valid_range");

  if(jit == jattribute.MemberEnd())
    throw eows::parse_error("File '" EOWS_GEOARRAYS_FILE "' is not valid.");
  
  attr.valid_range = read_valid_range(jit->value);

  attr.scale_factor = eows::core::read_node_as_double(jattribute, "scale_factor");
  attr.missing_value = eows::core::read_node_as_double(jattribute, "missing_value");;
  
  return attr;
}

eows::geoarray::dimensions_t
eows::geoarray::read_dimensions(const rapidjson::Value& jdimensions)
{
  if(!jdimensions.IsObject())
    throw eows::parse_error("File '" EOWS_GEOARRAYS_FILE "' is not valid.");
  
  dimensions_t dims;
  
  rapidjson::Value::ConstMemberIterator jit = jdimensions.FindMember("x");

  if(jit == jdimensions.MemberEnd())
    throw eows::parse_error("File '" EOWS_GEOARRAYS_FILE "' is not valid.");
  
  dims.x = read_dimension(jit->value);
  
  jit = jdimensions.FindMember("y");

  if(jit == jdimensions.MemberEnd())
    throw eows::parse_error("File '" EOWS_GEOARRAYS_FILE "' is not valid.");
  
  dims.y = read_dimension(jit->value);
  
  jit = jdimensions.FindMember("t");

  if(jit == jdimensions.MemberEnd())
    throw eows::parse_error("File '" EOWS_GEOARRAYS_FILE "' is not valid.");
  
  dims.t = read_dimension(jit->value);
  
  return dims;
}

eows::geoarray::dimension_t
eows::geoarray::read_dimension(const rapidjson::Value& jdimension)
{
  if(!jdimension.IsObject())
    throw eows::parse_error("File '" EOWS_GEOARRAYS_FILE "' is not valid.");
  
  dimension_t dim;

  dim.name = eows::core::read_node_as_string(jdimension, "name");
  dim.min_idx = eows::core::read_node_as_int64(jdimension, "min_idx");
  dim.max_idx = eows::core::read_node_as_int64(jdimension, "max_idx");

  return dim;
}

eows::geoarray::spatial_extent_t
eows::geoarray::read_spatial_extent(const rapidjson::Value& jspatial_extent)
{
  if(!jspatial_extent.IsObject())
    throw eows::parse_error("File '" EOWS_GEOARRAYS_FILE "' is not valid.");

  spatial_extent_t ext;

  ext.xmin = eows::core::read_node_as_double(jspatial_extent, "xmin");
  ext.ymin = eows::core::read_node_as_double(jspatial_extent, "ymin");
  ext.xmax = eows::core::read_node_as_double(jspatial_extent, "xmax");
  ext.ymax = eows::core::read_node_as_double(jspatial_extent, "ymax");

  return ext;
}

eows::geoarray::numeric_range_t
eows::geoarray::read_valid_range(const rapidjson::Value& jvalid_range)
{
  if(!jvalid_range.IsObject())
    throw eows::parse_error("File '" EOWS_GEOARRAYS_FILE "' is not valid.");

  numeric_range_t nr;
  
  nr.min_val = eows::core::read_node_as_double(jvalid_range, "min");
  nr.max_val = eows::core::read_node_as_double(jvalid_range, "max");

  return nr;
}

eows::geoarray::timeline_t
eows::geoarray::read_timeline(const rapidjson::Value& jtimeline, const dimension_t& t)
{
  if(!jtimeline.IsArray())
    throw eows::parse_error("File '" EOWS_GEOARRAYS_FILE "' is not valid.");
  
  std::vector<std::string> timeline;
  
  for(rapidjson::SizeType i = 0; i < jtimeline.Size(); ++i)
    timeline.push_back(eows::core::read_node_as_string(jtimeline[i]));
  
  return timeline_t(timeline, t);
}

eows::geoarray::spatial_resolution_t
eows::geoarray::read_resolution(const rapidjson::Value& jresolution)
{
  if(!jresolution.IsObject())
    throw eows::parse_error("File '" EOWS_GEOARRAYS_FILE "' is not valid.");

  spatial_resolution_t res;

  res.x = eows::core::read_node_as_double(jresolution, "x");
  res.y = eows::core::read_node_as_double(jresolution, "y");

  return res;
}

eows::geoarray::internal_metadata_t
eows::geoarray::read_internal_metadata(const rapidjson::Value& jinternal_metadata)
{
  if(!jinternal_metadata.IsObject())
    throw eows::parse_error("File '" EOWS_GEOARRAYS_FILE "' is not valid.");

  internal_metadata_t im;

  rapidjson::Value::ConstMemberIterator jit = jinternal_metadata.FindMember("spatial_extent");

  if(jit == jinternal_metadata.MemberEnd())
    throw eows::parse_error("File '" EOWS_GEOARRAYS_FILE "' is not valid.");

  im.spatial_extent = read_spatial_extent(jit->value);

  jit = jinternal_metadata.FindMember("spatial_resolution");

  if(jit == jinternal_metadata.MemberEnd())
    throw eows::parse_error("File '" EOWS_GEOARRAYS_FILE "' is not valid.");

  im.spatial_resolution = read_resolution(jit->value);

  jit = jinternal_metadata.FindMember("srid");

  if((jit == jinternal_metadata.MemberEnd()) || (!jit->value.IsUint()))
    throw eows::parse_error("File '" EOWS_GEOARRAYS_FILE "' is not valid.");

  im.srid = jit->value.GetUint();

  return im;
}

static void load_geoarrays()
{
  boost::filesystem::path cfg_file(eows::core::app_settings::instance().get_base_dir());
  
  cfg_file /= EOWS_GEOARRAYS_FILE;
  
  EOWS_LOG_INFO("Reading file '" + cfg_file.string() + "'...");
  
  rapidjson::Document doc = eows::core::open_json_file(cfg_file.string());
  
  if(!doc.HasMember("arrays") || !doc.IsObject())
    throw eows::parse_error("Please, check key 'arrays' in file '" EOWS_GEOARRAYS_FILE "'.");
  
  const rapidjson::Value& jarrays = doc["arrays"];
  
  if(!jarrays.IsArray())
    throw eows::parse_error("Key 'arrays' in file '" EOWS_GEOARRAYS_FILE "' must be a valid JSON array of objects.");

  for(rapidjson::SizeType i = 0; i < jarrays.Size(); ++i)
  {
    eows::geoarray::geoarray_t geo_array = eows::geoarray::read_geo_array(jarrays[i]);

    if(!eows::proj4::srs_manager::instance().exists(geo_array.srid) ||
       !eows::proj4::srs_manager::instance().exists(geo_array.i_meta.srid))
    {
      boost::format err_msg("Skipping GeoArray '%1%': missing spatial reference system definition for srid '%2%' or '%3%'.");

      EOWS_LOG_WARN((err_msg % geo_array.name % geo_array.srid % geo_array.i_meta.srid).str());

      continue;
    }

    eows::geoarray::geoarray_manager::instance().insert(geo_array);
  }

  EOWS_LOG_INFO("Finished reading file '" + cfg_file.string() + "'.");
}

void eows::geoarray::initialize()
{
  EOWS_LOG_INFO("Initializing GeoArrays runtime module...");

  load_geoarrays();

  EOWS_LOG_INFO("GeoArrays runtime module initialized!");
}
