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
  \file eows/ogc/wcs/operations/get_coverage.cpp

  \brief Implementation of WCS GetCoverage operation

  \author Raphael Willian da Costa
 */

// EOWS
#include "get_coverage.hpp"
#include "data_types.hpp"
#include "../core/utils.hpp"

// EOWS Core
#include "../../../core/utils.hpp"
#include "../../../core/file_remover.hpp"
#include "../../../core/logger.hpp"

// EOWS GeoArray
#include "../../../geoarray/data_types.hpp"
#include "../../../geoarray/geoarray_manager.hpp"
#include "../../../geoarray/utils.hpp"

// EOWS SciDB
#include "../../../scidb/exception.hpp"
#include "../../../scidb/connection.hpp"
#include "../../../scidb/connection_pool.hpp"
#include "../../../scidb/cell_iterator.hpp"
#include "../../../scidb/scoped_query.hpp"

// EOWS Proj4
#include "../../../proj4/srs.hpp"

// EOWS GDAL
#include "../../../gdal/dataset_geotiff.hpp"

// STL
#include <fstream>

// RapidXML
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>

// SciDB
#include <SciDBAPI.h>

// Boost
#include <boost/shared_ptr.hpp>


thread_local eows::proj4::spatial_ref_map t_srs_idx;

// struct Impl implementation
struct eows::ogc::wcs::operations::get_coverage::impl
{
  impl(const eows::ogc::wcs::operations::get_coverage_request& req)
    : request(req), file_handler(new eows::core::file_remover), output()
  {
  }

  /*!
   * \brief It generates a SciDB Query AFL to retrieve GetCoverage
   * \param array - Current geoarray
   * \param dimensions - Client subsets dimensions
   * \return A SciDB AFL query
   */
  std::string generate_afl(const eows::geoarray::geoarray_t& array,
                           const std::vector<eows::geoarray::dimension_t> dimensions);

  /*!
   * \brief It reprojects a dimension in lat long mode to Grid scale mode in order to retrieve correct array values
   *
   * \param array - Geo array with dimensions
   * \param srid - SRID to convert
   * \param x - Value of X to reproject
   * \param y - Value of Y to reproject
   */
  void reproject(const eows::geoarray::geoarray_t& array, const std::size_t& srid, double& x, double& y);

  /*!
   * \brief It performs subset reprojection if client gives a SRID different from Geoarray native. After that, it checks
   * if latitude/longitude reprojected intersects with geoarray. Throws exception when it does not intersects.
   *
   * \param array - Geo array with meta information
   * \param latitude - Latitude value
   * \param longitude - Longitude value
   * \throws eows::ogc::wcs::invalid_axis_error When there is any axis invalid and dont intersects
   */
  void validate(const eows::geoarray::geoarray_t& array, double& latitude, double& longitude);

  /*!
   * \brief It retrieves all subsets given by client. Once listed, it performs a subset validation for each one to provide a
   * correct projection values and limits. Remember that it retrieves default values even if passing a single value.
   * \param grid_array - geo array metadata
   * \param extent - Extent based in client subset
   * \return Array of subsets converted to grid dimensions
   */
  std::vector<eows::geoarray::dimension_t> retrieve_subsets(const eows::geoarray::grid& grid_array, geoarray::spatial_extent_t& extent);

  /*!
   * \brief It retrieves all range subset attributes given by client. If no set, retrieve array defaults.
   * \throws eows::ogc::wcs::no_such_field_error When client attributes does not match with array attribute
   * \param array - geo array
   * \return Client given attributes or array attributes
   */
  std::vector<eows::geoarray::attribute_t> retrieve_attributes(const eows::geoarray::geoarray_t& array);

  /*!
   * \brief It reads a SciDB query result as GML document. Once read, it prepares a WCS Coverage XML element with meta result
   * \param array - Current Geo Array
   * \param cell_it - SciDB Query Result
   * \param array_attributes - SciDB Geo Array Attributes
   * \param used_extent - Array extent used for retrieving SciDB data
   * \param dimensions_query - Dimensions used to fetch query
   */
  void process_as_document(const eows::geoarray::geoarray_t& array,
                           boost::shared_ptr<eows::scidb::cell_iterator> cell_it,
                           const ::scidb::Attributes& array_attributes,
                           const eows::geoarray::spatial_extent_t& used_extent,
                           const std::vector<geoarray::dimension_t> dimensions_query);

  void process_as_tiff(boost::shared_ptr<eows::scidb::cell_iterator> cell_it,
                       const eows::geoarray::geoarray_t& array,
                       const ::scidb::Attributes& attributes,
                       const std::vector<eows::geoarray::dimension_t> dimensions);

  //!< Represents WCS client arguments given. TODO: Use it as smart-pointer instead a const value
  const eows::ogc::wcs::operations::get_coverage_request request;
  //!< Represents a cast of array/client subset in lat/long mode to Grid scale mode based in SRID
  std::vector<eows::geoarray::dimension_t> grid_subset;
  //!< Represents Auto File remover for Image generation
  eows::core::file_remover_ptr file_handler;
  //!< Represents WCS GetCoverage output in GML format.
  std::string output;
};

void eows::ogc::wcs::operations::get_coverage::impl::reproject(const eows::geoarray::geoarray_t& array,
                                                               const std::size_t& srid,
                                                               double& x,
                                                               double& y)
{
  eows::proj4::spatial_reference* src_srs = nullptr;
  eows::proj4::spatial_reference* dst_srs = nullptr;

  // First of all, we must reproject client subsets to an compatible array
  eows::proj4::spatial_ref_map::const_iterator it = t_srs_idx.find(request.input_crs);

  // If it already indexed
  if(it != t_srs_idx.end())
    src_srs = it->second.get();
  else
  {
    // Retrieving GeoArray SRS (dst)
    const eows::proj4::srs_description_t& srs_desc = eows::proj4::srs_manager::instance().get(request.input_crs);
    std::unique_ptr<eows::proj4::spatial_reference> srs(new eows::proj4::spatial_reference(srs_desc.proj4_txt));

    src_srs = srs.get();
    t_srs_idx.insert(std::make_pair(request.input_crs, std::move(srs)));
  }

  eows::proj4::spatial_ref_map::const_iterator it_srid_target = t_srs_idx.find(array.i_meta.srid);

  if(it_srid_target != t_srs_idx.end())
    dst_srs = it_srid_target->second.get();
  else
  {
    const eows::proj4::srs_description_t& dst_desc = eows::proj4::srs_manager::instance().get(array.i_meta.srid);

    std::unique_ptr<eows::proj4::spatial_reference> srs(new eows::proj4::spatial_reference(dst_desc.proj4_txt));

    dst_srs = srs.get();

    t_srs_idx.insert(std::make_pair(array.i_meta.srid, std::move(srs)));
  }

  eows::proj4::transform(*src_srs, *dst_srs, x, y);
}

void eows::ogc::wcs::operations::get_coverage::impl::process_as_tiff(boost::shared_ptr<eows::scidb::cell_iterator> cell_it,
                                                                     const eows::geoarray::geoarray_t& array,
                                                                     const ::scidb::Attributes& attributes,
                                                                     const std::vector<eows::geoarray::dimension_t> dimensions)
{
  const std::string tmp_file_path = "/tmp/" + eows::core::generate_unique_path() + ".tiff";

  const eows::geoarray::dimension_t& dimension_x = dimensions[0];
  const eows::geoarray::dimension_t& dimension_y = dimensions[0];

  const int x = dimension_x.max_idx - dimension_x.min_idx + 1;
  const int y = dimension_y.max_idx - dimension_y.min_idx + 1;

  // TODO: Get array limits (from scidb query or input parameters?)
  eows::gdal::dataset_geotiff file(tmp_file_path, x, y, array.attributes.size());

  // Adding to File_remover
  file_handler->add(tmp_file_path);

  std::unordered_map<std::string, std::vector<double>> field_values;

  while(!cell_it->end())
  {
    for(const ::scidb::AttributeDesc& attribute: attributes)
    {
      const std::string& name = attribute.getName();
      const ::scidb::TypeId& data_type = attribute.getType();
      if (data_type == ::scidb::TID_INT16)
        field_values[name].push_back(cell_it->get_int16(name));
      else if (data_type == ::scidb::TID_UINT16)
        field_values[name].push_back(cell_it->get_uint16(name));
      else if (data_type == ::scidb::TID_INT8)
        field_values[name].push_back(cell_it->get_int8(name));
      else if (data_type == ::scidb::TID_UINT8)
        field_values[name].push_back(cell_it->get_uint8(name));
      else if (data_type == ::scidb::TID_INT32)
        field_values[name].push_back(cell_it->get_int32(name));
      else
        throw eows::ogc::wcs::no_such_field_error("Invalid attribute type");
    }

    cell_it->next();
  }

  int bid = 1;
  for(auto& it: field_values)
  {
    file.write(it.second, bid);
    ++bid;
  }
  // Close dataset
  file.close();
  // Opening generated image
  std::ifstream stream(tmp_file_path, std::ios::binary);
  // Reading Image binary to string
  output = std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
  // Closing file
  stream.close();
}

std::string eows::ogc::wcs::operations::get_coverage::impl::generate_afl(const eows::geoarray::geoarray_t& array,
                                                                         const std::vector<eows::geoarray::dimension_t> dimensions)
{
  // Preparing SciDB query string
  std::string query_str = "between(" + array.name + ", ";

  // Defining helpers for AFL Query generation
  std::string min_values;
  std::string max_values;

  for(const eows::geoarray::dimension_t& dimension: dimensions)
  {
    min_values += std::to_string(dimension.min_idx) + ",";
    max_values += std::to_string(dimension.max_idx) + ",";
  }
  // Removing extra comma
  min_values.pop_back();
  max_values.pop_back();

  // Generating SciDB AFL statement
  query_str +=  min_values + ", " + max_values + ")";

  // attributes_afl
  std::string attributes_afl;

  // Retrieve client attributes or array defaults
  std::vector<eows::geoarray::attribute_t> attributes = retrieve_attributes(array);

  for(auto& attribute: attributes)
    attributes_afl += attribute.name + ",";

  // remove last char (",") from attributes_afl
  attributes_afl.pop_back();

  query_str = "project(" + query_str + ", " + attributes_afl + ")";

  return query_str;
}

void eows::ogc::wcs::operations::get_coverage::impl::validate(const eows::geoarray::geoarray_t& array,
                                                              double& latitude,
                                                              double& longitude)
{
  // Performs re-projection of values if the SRID are different
  if (request.input_crs != array.srid)
    reproject(array, request.input_crs, latitude, longitude);

  // Validate Limits
  if (!array.spatial_extent.intersects(latitude, longitude))
    throw eows::ogc::wcs::invalid_axis_error("Values does not intersects");
}

std::vector<eows::geoarray::dimension_t>
eows::ogc::wcs::operations::get_coverage::impl::retrieve_subsets(const eows::geoarray::grid& grid_array,
                                                                 eows::geoarray::spatial_extent_t& extent)
{
  // Defining three dimensions fixed. TODO: We should get it from array.dimensions.size() if it were array
  std::vector<eows::geoarray::dimension_t> output;
  // Appending defaults values in order
  output.push_back(grid_array.geo_array->dimensions.x);
  output.push_back(grid_array.geo_array->dimensions.y);
  output.push_back(grid_array.geo_array->dimensions.t);

  // If WCS client gives subset limits, use them
  if (request.subsets.size() > 0)
  {
    // Now, we offer coverage 3 dimensions. In this case, we formed harded-code with this value to process GetCoverage. TODO: change it
    for(const eows::ogc::wcs::core::subset_t& client_subset: request.subsets)
    {
      double latitude = client_subset.min;
      double longitude = client_subset.max;
      // Col_id
      if(client_subset.name == grid_array.geo_array->dimensions.x.name)
      {
        validate(*(grid_array.geo_array), latitude, longitude);

        extent.xmin = latitude;
        extent.xmax = longitude;

        eows::geoarray::dimension_t d;
        d.name = client_subset.name;
        d.min_idx = grid_array.col(latitude);
        d.max_idx = grid_array.col(longitude);
        output[0] = d;
      }
      // Row ID
      else if(client_subset.name == grid_array.geo_array->dimensions.y.name)
      {
        validate(*(grid_array.geo_array), latitude, longitude);

        extent.ymin = latitude;
        extent.ymax = longitude;

        eows::geoarray::dimension_t d;
        d.name = client_subset.name;
        d.min_idx = grid_array.row(latitude);
        d.max_idx = grid_array.row(longitude);
        output[1] = d;
      }
      // Time ID
      else if(client_subset.name == grid_array.geo_array->dimensions.t.name)
      {
        if (client_subset.min < grid_array.geo_array->dimensions.t.min_idx ||
            client_subset.max > grid_array.geo_array->dimensions.t.max_idx)
          throw eows::ogc::wcs::invalid_axis_error("Time axis does not intersects. " + client_subset.name);

        eows::geoarray::dimension_t time_dimension;
        time_dimension.name = client_subset.name;
        time_dimension.min_idx = client_subset.min;
        time_dimension.max_idx = client_subset.max;
        output[2] = time_dimension;
      }
      else
        throw eows::ogc::wcs::invalid_axis_error("No axis found. " + client_subset.name);
    } // end for
  } // end if subsets.size()

  return output;
}

void eows::ogc::wcs::operations::get_coverage::impl::process_as_document(const eows::geoarray::geoarray_t& array,
                                                                         boost::shared_ptr<eows::scidb::cell_iterator> cell_it,
                                                                         const ::scidb::Attributes& array_attributes,
                                                                         const eows::geoarray::spatial_extent_t& used_extent,
                                                                         const std::vector<eows::geoarray::dimension_t> dimensions_query)
{
  // Defining output stream where SciDB will be stored (GML format)
  std::ostringstream ss;
  // Retrieving Array size
  auto attributes_size = array_attributes.size();
  // Delimiter used in GML generation
  const std::string row_delimiter(",");
  const std::string attr_delimiter(" ");
  /*
    It will generate SciDB data output in GML syntax.
    Note: You may change delimiters as you need, but remember to update GML output.

    data0_attr1 data0_attr2 data0_attrN,dataN_attr1 dataN_attr2 dataN_attrN,...
  */
  while(!cell_it->end())
  {
    for(std::size_t attr_pos = 0; attr_pos < attributes_size; ++attr_pos)
    {
      const ::scidb::AttributeDesc& attr_scidb = array_attributes[attr_pos];

      const ::scidb::TypeId& type_id = attr_scidb.getType();

      if (type_id == ::scidb::TID_INT16)
        ss << cell_it->get_int16(attr_scidb.getName());
      else if (type_id == ::scidb::TID_UINT8)
        ss << cell_it->get_uint8(attr_scidb.getName());
      else if (type_id == ::scidb::TID_INT8)
        ss << std::to_string(cell_it->get_int8(attr_scidb.getName()));

      // TODO: Remove this check. It should append and remove last char on finish attr
      if (attr_pos + 1 < attributes_size)
        ss << attr_delimiter;
    }
    ss << row_delimiter;
    cell_it->next();
  }

  // Defining GetCoverage XML document
  rapidxml::xml_document<> xml_doc;

  // Preparing Meta document
  rapidxml::xml_node<>* decl = xml_doc.allocate_node(rapidxml::node_declaration);
  decl->append_attribute(xml_doc.allocate_attribute("version", "1.0"));
  decl->append_attribute(xml_doc.allocate_attribute("encoding", "UTF-8"));
  xml_doc.append_node(decl);
  // Preparing WCS Root Element
  rapidxml::xml_node<>* wcs_document = xml_doc.allocate_node(rapidxml::node_element, "gmlcov:GridCoverage");
  wcs_document->append_attribute(xml_doc.allocate_attribute("version", "2.0.1"));
  wcs_document->append_attribute(xml_doc.allocate_attribute("xmlns:gml","http://www.opengis.net/gml/3.2"));
  wcs_document->append_attribute(xml_doc.allocate_attribute("xmlns","http://www.opengis.net/gml/3.2"));
  wcs_document->append_attribute(xml_doc.allocate_attribute("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance"));
  wcs_document->append_attribute(xml_doc.allocate_attribute("xmlns:gmlcov", "http://www.opengis.net/gmlcov/1.0"));
  wcs_document->append_attribute(xml_doc.allocate_attribute("xmlns:swe", "http://www.opengis.net/swe/2.0"));
  wcs_document->append_attribute(xml_doc.allocate_attribute("xsi:schemaLocation",
                                                            "http://www.opengis.net/swe/2.0 http://schemas.opengis.net/sweCommon/2.0/swe.xsd http://www.opengis.net/gmlcov/1.0 http://schemas.opengis.net/gmlcov/1.0/gmlcovAll.xsd"));
  wcs_document->append_attribute(xml_doc.allocate_attribute("gml:id", array.name.c_str()));
  xml_doc.append_node(wcs_document);

  // Preparing bounded by
  eows::ogc::wcs::core::make_coverage_bounded_by(&xml_doc, wcs_document, array, used_extent);
  // Preparing domainset
  eows::ogc::wcs::core::make_coverage_domain_set(&xml_doc, wcs_document, array, dimensions_query);

  // Preparing rangeset
  rapidxml::xml_node<>* range_set = xml_doc.allocate_node(rapidxml::node_element, "gml:rangeSet");
  wcs_document->append_node(range_set);
  rapidxml::xml_node<>* data_block = xml_doc.allocate_node(rapidxml::node_element, "gml:DataBlock");

  const std::string scidb_data = ss.str();

  rapidxml::xml_node<>* tuple_list = xml_doc.allocate_node(rapidxml::node_element, "gml:tupleList", scidb_data.c_str(), 0, scidb_data.size());
  // Defining delimiter in order to client use to read properly row
  tuple_list->append_attribute(xml_doc.allocate_attribute("ts", row_delimiter.c_str()));
  // Defining delimiter for coverage attribute
  tuple_list->append_attribute(xml_doc.allocate_attribute("cs", attr_delimiter.c_str()));

  data_block->append_node(tuple_list);
  range_set->append_node(data_block);

  // Preparing rangetype
  eows::ogc::wcs::core::make_coverage_range_type(&xml_doc, wcs_document, retrieve_attributes(array));

  rapidxml::print(std::back_inserter(output), xml_doc, 0);
}

std::vector<eows::geoarray::attribute_t>
eows::ogc::wcs::operations::get_coverage::impl::retrieve_attributes(const eows::geoarray::geoarray_t& array)
{
  // Checking WCS RangeSubsetting
  if (!request.range_subset.attributes.empty())
  {
    std::vector<eows::geoarray::attribute_t> output;

    // For each given attribute
    for(std::size_t i = 0; i < request.range_subset.attributes.size(); ++i)
    {
      const std::string& given_attr = request.range_subset.attributes[i];

      auto found = std::find_if(array.attributes.begin(),
                                array.attributes.end(),
                                [&given_attr] (const eows::geoarray::attribute_t& array_attr) {
                                  return given_attr == array_attr.name;
                                });

      if (found == array.attributes.end())
        throw eows::ogc::wcs::no_such_field_error("No such field " + given_attr);

      output.push_back(*found);
    }

    return output;
  }
  else
    return array.attributes;
}

// GetCoverage Implementations

eows::ogc::wcs::operations::get_coverage::get_coverage(const eows::ogc::wcs::operations::get_coverage_request& req)
  : pimpl_(new impl(req))
{

}

eows::ogc::wcs::operations::get_coverage::~get_coverage()
{
  delete pimpl_;
}

void eows::ogc::wcs::operations::get_coverage::execute()
{
  try
  {
    // Retrieve GeoArray information
    const geoarray::geoarray_t& array = geoarray::geoarray_manager::instance().get(pimpl_->request.coverage_id);

    // Wrapping Geo Array as Grid type
    eows::geoarray::grid grid_array(&array);

    // Retrieving spatial extent (Default array)
    geoarray::spatial_extent_t used_extent = array.spatial_extent;

    // Array containing client limits sent in WCS request that will be used to build SciDB AFL query
    std::vector<eows::geoarray::dimension_t> dimensions_to_query = pimpl_->retrieve_subsets(grid_array, used_extent);

    // Get Query AFL
    std::string query_str = pimpl_->generate_afl(array, dimensions_to_query);

    // Open SciDB connection
    eows::scidb::connection conn = eows::scidb::connection_pool::instance().get(array.cluster_id);

    // Performing AFL query execution
    boost::shared_ptr<::scidb::QueryResult> query_result = conn.execute(query_str);
    // Wrapping SciDB result with Scoped query to auto complete query exec
    eows::scidb::scoped_query sc(query_result, &conn);

    // TODO: Improve err message when no data found or query error
    if((query_result == nullptr) || (query_result->array == nullptr))
      throw eows::scidb::query_execution_error("Error in SciDB query result");

    boost::shared_ptr<eows::scidb::cell_iterator> cell_it(new eows::scidb::cell_iterator(query_result->array));

    const ::scidb::ArrayDesc& array_desc = query_result->array->getArrayDesc();
    const ::scidb::Attributes& array_attributes = array_desc.getAttributes(true);

    /*
      Defining how to build GetCoverage based in Format.

      TODO: Create a factory handler
    */
    if (pimpl_->request.format == "application/gml+xml")
      pimpl_->process_as_document(array, std::move(cell_it), array_attributes, used_extent, dimensions_to_query);
    else if (pimpl_->request.format == "image/tiff")
      pimpl_->process_as_tiff(std::move(cell_it), array, array_attributes, dimensions_to_query);
  }
  // known module error
  catch(const eows::ogc::ogc_error&)
  {
    throw;
  }
  // thirdparty errors
  catch(const eows::scidb::connection_open_error& e)
  {
    // Todo: Throw UnappliedError
    throw eows::ogc::ogc_error(e.what(), "");
  }
  // STL errors
  catch(const std::invalid_argument& e) //thrown by geoarray::manager.get() when array not found
  {
    throw wcs::no_such_coverage_error("No such coverage '" + pimpl_->request.coverage_id + "'");
  }
  catch(const std::exception& e)
  {
    throw;
  }
}

const char*eows::ogc::wcs::operations::get_coverage::content_type() const
{
  return pimpl_->request.format.c_str();
}

const std::string& eows::ogc::wcs::operations::get_coverage::to_string() const
{
  return pimpl_->output;
}
