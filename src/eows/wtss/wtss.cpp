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
  \file eows/wtss/wtss.cpp

  \brief Web Time Series Service.

  \author Gilberto Ribeiro de Queiroz
 */

// EOWS
#include "wtss.hpp"
#include "../core/logger.hpp"
#include "../core/http_response.hpp"
#include "../core/http_request.hpp"
#include "../core/service_operations_manager.hpp"
#include "../core/utils.hpp"
#include "../geoarray/geoarray_manager.hpp"
#include "../geoarray/utils.hpp"
#include "../scidb/connection.hpp"
#include "../scidb/connection_pool.hpp"
#include "../scidb/cell_iterator.hpp"
#include "../scidb/scoped_query.hpp"
#include "../proj4/converter.hpp"

// Boost
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

// SciDB
#include <SciDBAPI.h>

// RapidJSON
#include <rapidjson/rapidjson.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

static void
return_exception(const char* msg, eows::core::http_response& res);

namespace eows
{
  namespace wtss
  {
    struct timeseries_request_parameters
    {
      std::string cv_name;
      std::vector<std::string> queried_attributes;
      double longitude;
      double latitude;
      std::string start_time_point;
      std::string end_time_point;
    };

    struct timeseries_validated_parameters
    {
      std::vector<std::size_t> attribute_positions;
      const eows::geoarray::geoarray_t* geo_array;
      std::pair<std::size_t, std::size_t> time_interval;
    };

    struct cell_location
    {
      double x;
      double y;
      double center_x;
      double center_y;
      double center_lon;
      double center_lat;
      int64_t col;
      int64_t row;
    };

    timeseries_request_parameters
    decode_timeseries_request(const eows::core::query_string_t& qstr);

    timeseries_validated_parameters
    valid(const timeseries_request_parameters& parameters);

    cell_location
    find_location(const double& longitude,
                  const double& latitude,
                  const eows::geoarray::geoarray_t* geo_array);

    void compute_time_series(const timeseries_request_parameters& parameters,
                             const timeseries_validated_parameters& vparameters,
                             const cell_location& cell,
                             rapidjson::Writer<rapidjson::StringBuffer>& writer);

    /*!
      \brief Fill the timeseries with cell values.

      \param values    A pre-allocated vector with at least nvalues.
      \param cell_it   EOWS cell iterator for SciDB query result
      \param id        The datatype of the cell.
      \param attr_name An attribute name of array

      \exception eows::outof_bounds_error If the number of values found is less than or greater than the number o expected time-series values.
    */
    void fill_time_series(std::vector<double>& values,
                          boost::shared_ptr<eows::scidb::cell_iterator> cell_it,
                          const ::scidb::TypeId& id,
                          const std::string& attr_name);

  }  // end namespace wtss
}    // end namespace eows

void
eows::wtss::list_coverages_handler::do_get(const eows::core::http_request& req,
                                           eows::core::http_response& res)
{
  std::vector<std::string> arrays = eows::geoarray::geoarray_manager::instance().list_arrays();

  rapidjson::StringBuffer buff;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buff);

  writer.StartObject();

  writer.Key("coverages", static_cast<rapidjson::SizeType>(sizeof("coverages") -1));
  eows::core::write_string_array(arrays.begin(), arrays.end(), writer);

  writer.EndObject();

  res.set_status(eows::core::http_response::OK);

  res.add_header(eows::core::http_response::CONTENT_TYPE, "application/json; charset=utf-8");
  res.add_header(eows::core::http_response::ACCESS_CONTROL_ALLOW_ORIGIN, "*");

  res.write(buff.GetString(), buff.GetSize());
}

void
eows::wtss::describe_coverage_handler::do_get(const eows::core::http_request& req,
                                              eows::core::http_response& res)
{
  try
  {
    eows::core::query_string_t qstr(req.query_string());

    eows::core::query_string_t::const_iterator it = qstr.find("name");

    if(it == qstr.end())
      throw std::runtime_error("Error in operation 'describe_coverage' for WTSS: missing coverage name.");

    const eows::geoarray::geoarray_t& geo_array = eows::geoarray::geoarray_manager::instance().get(it->second);

    rapidjson::StringBuffer buff;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buff);

    eows::geoarray::write(writer, geo_array);

    res.set_status(eows::core::http_response::OK);

    res.add_header(eows::core::http_response::CONTENT_TYPE, "application/json; charset=utf-8");
    res.add_header(eows::core::http_response::ACCESS_CONTROL_ALLOW_ORIGIN, "*");

    res.write(buff.GetString(), buff.GetSize());
  }
  catch(const std::exception& e)
  {
    return_exception(e.what(), res);
  }
  catch(...)
  {
    return_exception("Unexpected error in WTSS describe_coverage operation.", res);
  }
}

void
eows::wtss::time_series_handler::do_get(const eows::core::http_request& req,
                                        eows::core::http_response& res)
{
  try
  {
    eows::core::query_string_t qstr(req.query_string());

    timeseries_request_parameters parameters = decode_timeseries_request(qstr);

    timeseries_validated_parameters vparameters = valid(parameters);

    cell_location cell = find_location(parameters.longitude,
                                       parameters.latitude,
                                       vparameters.geo_array);

    rapidjson::StringBuffer buff;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buff);

    writer.StartObject();

    writer.Key("result", static_cast<rapidjson::SizeType>(sizeof("result") -1));

    writer.StartObject();

    writer.Key("attributes", static_cast<rapidjson::SizeType>(sizeof("attributes") -1));

    compute_time_series(parameters, vparameters, cell, writer);

    writer.Key("timeline", static_cast<rapidjson::SizeType>(sizeof("timeline") -1));
    eows::core::write_string_array(std::begin(vparameters.geo_array->timeline.time_points()) + vparameters.time_interval.first,
                                   std::begin(vparameters.geo_array->timeline.time_points()) + (vparameters.time_interval.second + 1),
                                   writer);

    writer.Key("coordinates", static_cast<rapidjson::SizeType>(sizeof("coordinates") -1));

    writer.StartObject();

    writer.Key("longitude", static_cast<rapidjson::SizeType>(sizeof("longitude") -1));
    writer.Double(cell.center_lon);

    writer.Key("latitude", static_cast<rapidjson::SizeType>(sizeof("latitude") -1));
    writer.Double(cell.center_lat);

    writer.Key("col", static_cast<rapidjson::SizeType>(sizeof("col") -1));
    writer.Double(cell.col);

    writer.Key("row", static_cast<rapidjson::SizeType>(sizeof("row") -1));
    writer.Double(cell.row);

    writer.EndObject();  // coordinates

    writer.EndObject();  // result

    writer.Key("query", static_cast<rapidjson::SizeType>(sizeof("query") -1));

    writer.StartObject();

    writer.Key("coverage", static_cast<rapidjson::SizeType>(sizeof("coverage") -1));
    writer.String(parameters.cv_name.c_str(), static_cast<rapidjson::SizeType>(parameters.cv_name.length()));

    writer.Key("attributes", static_cast<rapidjson::SizeType>(sizeof("attributes") -1));
    eows::core::write_string_array(std::begin(parameters.queried_attributes),
                                   std::end(parameters.queried_attributes),
                                   writer);

    writer.Key("longitude", static_cast<rapidjson::SizeType>(sizeof("longitude") -1));
    writer.Double(parameters.longitude);

    writer.Key("latitude", static_cast<rapidjson::SizeType>(sizeof("latitude") -1));
    writer.Double(parameters.latitude);

    writer.Key("start_date", static_cast<rapidjson::SizeType>(sizeof("start_date") -1));
    writer.String(parameters.start_time_point.c_str(), static_cast<rapidjson::SizeType>(parameters.start_time_point.length()));

    writer.Key("end_date", static_cast<rapidjson::SizeType>(sizeof("end_date") -1));
    writer.String(parameters.end_time_point.c_str(), static_cast<rapidjson::SizeType>(parameters.end_time_point.length()));

    writer.EndObject();  // query


//    writer.Key("center_lon", static_cast<rapidjson::SizeType>(sizeof("center_lon") -1));
//    writer.Double(cell.center_lon);

//    writer.Key("center_lat", static_cast<rapidjson::SizeType>(sizeof("center_lat") -1));
//    writer.Double(cell.center_lat);

//    writer.Key("x", static_cast<rapidjson::SizeType>(sizeof("x") -1));
//    writer.Double(cell.x);

//    writer.Key("y", static_cast<rapidjson::SizeType>(sizeof("y") -1));
//    writer.Double(cell.y);

//    writer.Key("center_x", static_cast<rapidjson::SizeType>(sizeof("center_x") -1));
//    writer.Double(cell.center_x);

//    writer.Key("center_y", static_cast<rapidjson::SizeType>(sizeof("center_y") -1));
//    writer.Double(cell.center_y);

//    writer.Key("col", static_cast<rapidjson::SizeType>(sizeof("col") -1));
//    writer.Double(cell.col);

//    writer.Key("row", static_cast<rapidjson::SizeType>(sizeof("row") -1));
//    writer.Double(cell.row);

    writer.EndObject();

    res.set_status(eows::core::http_response::OK);

    res.add_header(eows::core::http_response::CONTENT_TYPE, "application/json; charset=utf-8");
    res.add_header(eows::core::http_response::ACCESS_CONTROL_ALLOW_ORIGIN, "*");

    res.write(buff.GetString(), buff.GetSize());
  }
  catch(const std::exception& e)
  {
    return_exception(e.what(), res);
  }
  catch(...)
  {
    return_exception("Unexpected error in WTSS time_series operation.", res);
  }
}

void
eows::wtss::location_handler::do_get(const eows::core::http_request& req,
                                     eows::core::http_response& res)
{
  try
  {
    eows::core::query_string_t qstr(req.query_string());

    timeseries_request_parameters parameters = decode_timeseries_request(qstr);

    timeseries_validated_parameters vparameters = valid(parameters);

    cell_location cell = find_location(parameters.longitude,
                                       parameters.latitude,
                                       vparameters.geo_array);

    rapidjson::StringBuffer buff;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buff);

    writer.StartObject();

    writer.Key("longitude", static_cast<rapidjson::SizeType>(sizeof("longitude") -1));
    writer.Double(parameters.longitude);

    writer.Key("latitude", static_cast<rapidjson::SizeType>(sizeof("latitude") -1));
    writer.Double(parameters.latitude);

    writer.Key("center_lon", static_cast<rapidjson::SizeType>(sizeof("center_lon") -1));
    writer.Double(cell.center_lon);

    writer.Key("center_lat", static_cast<rapidjson::SizeType>(sizeof("center_lat") -1));
    writer.Double(cell.center_lat);

    writer.Key("x", static_cast<rapidjson::SizeType>(sizeof("x") -1));
    writer.Double(cell.x);

    writer.Key("y", static_cast<rapidjson::SizeType>(sizeof("y") -1));
    writer.Double(cell.y);

    writer.Key("center_x", static_cast<rapidjson::SizeType>(sizeof("center_x") -1));
    writer.Double(cell.center_x);

    writer.Key("center_y", static_cast<rapidjson::SizeType>(sizeof("center_y") -1));
    writer.Double(cell.center_y);

    writer.Key("col", static_cast<rapidjson::SizeType>(sizeof("col") -1));
    writer.Double(cell.col);

    writer.Key("row", static_cast<rapidjson::SizeType>(sizeof("row") -1));
    writer.Double(cell.row);

    writer.Key("crs_out", static_cast<rapidjson::SizeType>(sizeof("crs_out") -1));

    writer.StartObject();

    const eows::proj4::srs_description_t& crs_out = eows::proj4::srs_manager::instance().get(vparameters.geo_array->srid);

    writer.Key("proj4", static_cast<rapidjson::SizeType>(sizeof("proj4") - 1));
    writer.String(crs_out.proj4_txt.c_str(), static_cast<rapidjson::SizeType>(crs_out.proj4_txt.length()));

    writer.Key("wkt", static_cast<rapidjson::SizeType>(sizeof("wkt") - 1));
    writer.String(crs_out.wkt.c_str(), static_cast<rapidjson::SizeType>(crs_out.wkt.length()));

    writer.EndObject();

    writer.Key("crs_in", static_cast<rapidjson::SizeType>(sizeof("crs_in") -1));

    writer.StartObject();

    const eows::proj4::srs_description_t& crs_in = eows::proj4::srs_manager::instance().get(vparameters.geo_array->i_meta.srid);

    writer.Key("proj4", static_cast<rapidjson::SizeType>(sizeof("proj4") - 1));
    writer.String(crs_in.proj4_txt.c_str(), static_cast<rapidjson::SizeType>(crs_in.proj4_txt.length()));

    writer.Key("wkt", static_cast<rapidjson::SizeType>(sizeof("wkt") - 1));
    writer.String(crs_in.wkt.c_str(), static_cast<rapidjson::SizeType>(crs_in.wkt.length()));

    writer.EndObject();

    writer.EndObject();

    res.set_status(eows::core::http_response::OK);

    res.add_header(eows::core::http_response::CONTENT_TYPE, "application/json; charset=utf-8");
    res.add_header(eows::core::http_response::ACCESS_CONTROL_ALLOW_ORIGIN, "*");

    res.write(buff.GetString(), buff.GetSize());
  }
  catch(const std::exception& e)
  {
    return_exception(e.what(), res);
  }
  catch(...)
  {
    return_exception("Unexpected error in WTSS location operation.", res);
  }
}

static void register_operations()
{
  EOWS_LOG_INFO("Registering WTSS operations...");

  std::unique_ptr<eows::wtss::list_coverages_handler> lc_h(new eows::wtss::list_coverages_handler);
  eows::core::service_operations_manager::instance().insert("/wtss/list_coverages", std::move(lc_h));

  std::unique_ptr<eows::wtss::describe_coverage_handler> dc_h(new eows::wtss::describe_coverage_handler);
  eows::core::service_operations_manager::instance().insert("/wtss/describe_coverage", std::move(dc_h));

  std::unique_ptr<eows::wtss::time_series_handler> ts_h(new eows::wtss::time_series_handler);
  eows::core::service_operations_manager::instance().insert("/wtss/time_series", std::move(ts_h));

  std::unique_ptr<eows::wtss::location_handler> l_h(new eows::wtss::location_handler);
  eows::core::service_operations_manager::instance().insert("/wtss/location", std::move(l_h));

  EOWS_LOG_INFO("WTSS operations registered!");
}

static void initialize_operations()
{

}

void
eows::wtss::initialize()
{
  EOWS_LOG_INFO("Initializing WTSS...");

  register_operations();

  initialize_operations();

  EOWS_LOG_INFO("WTSS initialized!");
}

void
return_exception(const char* msg, eows::core::http_response& res)
{
  rapidjson::StringBuffer buff;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buff);

  writer.StartObject();

  writer.Key("exception", static_cast<rapidjson::SizeType>(sizeof("exception") -1));
  writer.String(msg);

  writer.EndObject();

  res.set_status(eows::core::http_response::OK);

  res.add_header(eows::core::http_response::CONTENT_TYPE, "application/json; charset=utf-8");
  res.add_header(eows::core::http_response::ACCESS_CONTROL_ALLOW_ORIGIN, "*");

  res.write(buff.GetString(), buff.GetSize());
}

eows::wtss::timeseries_request_parameters
eows::wtss::decode_timeseries_request(const eows::core::query_string_t& qstr)
{
  timeseries_request_parameters parameters;

// get coverage name
  eows::core::query_string_t::const_iterator it = qstr.find("coverage");
  eows::core::query_string_t::const_iterator it_end = qstr.end();

  if(it == it_end)
    throw std::invalid_argument("WTSS 'time_series' operation error: \"coverage\" parameter is missing.");

  parameters.cv_name = it->second;

// get queried attributes
  it = qstr.find("attributes");

  if(it == it_end)
  {
    boost::format err_msg("WTSS 'time_series' operation error: \"attributes\" parameter is missing for coverage '%1%'.");
    throw std::invalid_argument((err_msg % parameters.cv_name).str());
  }

  boost::split(parameters.queried_attributes, it->second, boost::is_any_of(","));

// extract longitude
  it = qstr.find("longitude");

  if(it == it_end)
    throw std::invalid_argument("WTSS 'time_series' operation error: \"longitude\" parameter is missing.");

  parameters.longitude = boost::lexical_cast<double>(it->second);

// extract latitude
  it = qstr.find("latitude");

  if(it == it_end)
    throw std::invalid_argument("WTSS 'time_series' operation error: \"latitude\" parameter is missing.");

  parameters.latitude = boost::lexical_cast<double>(it->second);

// extract start and end times if any
  it = qstr.find("start_date");

  parameters.start_time_point = (it != it_end) ? it->second : std::string("");

  it = qstr.find("end_date");

  parameters.end_time_point = (it != it_end) ? it->second : std::string("");

// ok: finished extracting parameters
  return parameters;
}

eows::wtss::timeseries_validated_parameters
eows::wtss::valid(const timeseries_request_parameters& parameters)
{
  timeseries_validated_parameters vparameters;

// retrieve the underlying geoarray
  vparameters.geo_array = &(eows::geoarray::geoarray_manager::instance().get(parameters.cv_name));

// valid queried attributes
  if(parameters.queried_attributes.empty())
  {
    boost::format err_msg("WTSS 'time_series' operation error: please, inform at least one attribute for coverage '%1%'.");
    throw std::invalid_argument((err_msg % parameters.cv_name).str());
  }

  for(const std::string& attr_name : parameters.queried_attributes)
  {
    const std::vector<eows::geoarray::attribute_t>::const_iterator it_end = vparameters.geo_array->attributes.end();

    const std::vector<eows::geoarray::attribute_t>::const_iterator it = std::find_if(vparameters.geo_array->attributes.begin(),
                                                                                     it_end,
                                                                                     [&attr_name](const eows::geoarray::attribute_t& attr) -> bool { return (attr.name == attr_name); });

    if(it == it_end)
    {
      boost::format err_msg("WTSS 'time_series' operation error: attribute '%1%' doesn't belong to coverage '%2%'.");
      throw std::invalid_argument((err_msg % attr_name % parameters.cv_name).str());
    }

    std::size_t pos = std::distance(vparameters.geo_array->attributes.begin(), it);

    vparameters.attribute_positions.push_back(pos);
  }

// find and valid queried time-interval if one is provided
  vparameters.time_interval = vparameters.geo_array->timeline.find_interval(parameters.start_time_point, parameters.end_time_point);

// valid input coordinates
  if(!vparameters.geo_array->spatial_extent.intersects(parameters.longitude, parameters.latitude))
  {
    boost::format err_msg("WTSS 'time_series' operation error: longitude '%1%' or latitude '%2%' is out of range.");
    throw std::out_of_range((err_msg % parameters.longitude % parameters.latitude).str());
  }

  return vparameters;
}

eows::wtss::cell_location
eows::wtss::find_location(const double& longitude,
                          const double& latitude,
                          const eows::geoarray::geoarray_t* geo_array)
{
  cell_location cell;

  cell.x = longitude;
  cell.y = latitude;

  // Defining converter handler
  eows::proj4::converter converter;
  converter.set_source_srid(4326);
  converter.set_target_srid(geo_array->i_meta.srid);

// do we need to make coordinate transformation?
  if(geo_array->i_meta.srid != 4326)
  {
    // ok! we have both SRS handles.
    // let's perform the coordinate transformation
    converter.convert(cell.x, cell.y);
  }

// is the queried coordinate in array projection inside the real array extent?
  if(!geo_array->i_meta.spatial_extent.intersects(cell.x, cell.y))
  {
    boost::format err_msg("WTSS 'time_series' operation error: computed coordinate (%1%, %2%) is out of range.");
    throw std::out_of_range((err_msg % cell.x % cell.y).str());
  }

// compute the cell coordinate
  eows::geoarray::grid g(geo_array);

  cell.col = g.col(cell.x);
  cell.row = g.row(cell.y);
  cell.center_x = g.x(cell.col);
  cell.center_y = g.y(cell.row);
  cell.center_lon = cell.center_x;
  cell.center_lat = cell.center_y;

  //double dcol = (cell.x - geo_array->i_meta.spatial_extent.xmin) / geo_array->i_meta.spatial_resolution.x;
  //double drow = (geo_array->i_meta.spatial_extent.ymax - cell.y) / geo_array->i_meta.spatial_resolution.y;

  //cell.col = std::round(dcol) + geo_array->dimensions.x.min_idx;
  //cell.row = std::round(drow) + geo_array->dimensions.y.min_idx;

  //double half_res_x = geo_array->i_meta.spatial_resolution.x / 2.0;
  //double half_res_y = geo_array->i_meta.spatial_resolution.y / 2.0;

// compute the center of the pixel
  //cell.center_x = (static_cast<double>(cell.col) * geo_array->i_meta.spatial_resolution.x) + half_res_x + geo_array->i_meta.spatial_extent.xmin;
  //cell.center_y = geo_array->i_meta.spatial_extent.ymax - (static_cast<double>(cell.row) * geo_array->i_meta.spatial_resolution.y) - half_res_y;

  //cell.center_lon = cell.center_x;
  //cell.center_lat = cell.center_y;

// do we need to make the inverse coordinate transformation?
  if(geo_array->i_meta.srid != 4326)
    converter.convert(cell.center_lon, cell.center_lat);

  return cell;
}

void
eows::wtss::compute_time_series(const timeseries_request_parameters& parameters,
                                const timeseries_validated_parameters& vparameters,
                                const cell_location& cell,
                                rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
  writer.StartArray();

  const std::size_t ntime_pts = vparameters.time_interval.second - vparameters.time_interval.first + 1;

  const std::size_t nattributes = parameters.queried_attributes.size();

// get a connection from the pool in order to retrieve the time series data
  eows::scidb::connection conn(eows::scidb::connection_pool::instance().get(vparameters.geo_array->cluster_id));

// iterate through each queried attribute
  for(std::size_t i = 0; i != nattributes; ++i)
  {
    const auto& attr_name = parameters.queried_attributes[i];
    const std::size_t& attr_pos = vparameters.attribute_positions[i];

// the query string    
    std::string str_afl = "project( between(" + parameters.cv_name + ", "
                        + std::to_string(cell.col) + "," + std::to_string(cell.row) + "," + std::to_string(vparameters.time_interval.first) + ","
                        + std::to_string(cell.col) + "," + std::to_string(cell.row) + "," + std::to_string(vparameters.time_interval.second) + "), "
                        + attr_name + ")";

    boost::shared_ptr< ::scidb::QueryResult > qresult = conn.execute(str_afl);

    eows::scidb::scoped_query sc(qresult, &conn);

    if((qresult == nullptr) || (qresult->array == nullptr))
    {
      writer.StartObject();

      writer.Key("attribute", static_cast<rapidjson::SizeType>(sizeof("attribute") -1));
      writer.String(attr_name.c_str(), static_cast<rapidjson::SizeType>(attr_name.length()));

      writer.Key("values", static_cast<rapidjson::SizeType>(sizeof("values") -1));
      writer.Null();

      writer.EndObject();

      continue; // no query result returned after querying database.
    }

    boost::shared_ptr<eows::scidb::cell_iterator> cell_it(new eows::scidb::cell_iterator(qresult->array));

    std::vector<double> values(ntime_pts, vparameters.geo_array->attributes[attr_pos].missing_value);

    const ::scidb::ArrayDesc& array_desc = qresult->array->getArrayDesc();
    const ::scidb::Attributes& array_attributes = array_desc.getAttributes(true);
    const ::scidb::AttributeDesc& attr = array_attributes.front();

    auto attr_type = attr.getType();

// TODO: remover o valor constante 2 abaixo pela coluna temporal!
    fill_time_series(values, cell_it, attr.getType(), attr_name);

    writer.StartObject();

    writer.Key("attribute", static_cast<rapidjson::SizeType>(sizeof("attribute") -1));
    writer.String(attr_name.c_str(), static_cast<rapidjson::SizeType>(attr_name.length()));

    writer.Key("values", static_cast<rapidjson::SizeType>(sizeof("values") -1));
    eows::core::write_numeric_array(std::begin(values), std::end(values), writer);

    writer.EndObject();
  }

  writer.EndArray();
}

void
eows::wtss::fill_time_series(std::vector<double>& values,
                             boost::shared_ptr<eows::scidb::cell_iterator> cell_it,
                             const ::scidb::TypeId& id,
                             const std::string& attr_name)
{
  assert(cell_it);

  // TODO: Should pass a functor to read data to avoid conditional checks for type?
  while(!cell_it->end())
  {
    if (id == ::scidb::TID_INT8)
      values.push_back(cell_it->get_int8(attr_name));
    else if(id == ::scidb::TID_UINT8)
      values.push_back(cell_it->get_uint8(attr_name));
    else if(id == ::scidb::TID_INT16)
      values.push_back(cell_it->get_int16(attr_name));
    else if(id == ::scidb::TID_UINT16)
      values.push_back(cell_it->get_uint16(attr_name));
    else if(id == ::scidb::TID_INT32)
      values.push_back(cell_it->get_int32(attr_name));
    else if(id == ::scidb::TID_INT32)
      values.push_back(cell_it->get_int32(attr_name));
    else
      throw std::runtime_error("Could not fill values vector with iterator items: data type not supported.");

    cell_it->next();
  }
}
