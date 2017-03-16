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
  \file eows/ogc/wms/data_types.hpp

  \brief Datatypes for WMS.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_OGC_WMS_DATA_TYPES_HPP__
#define __EOWS_OGC_WMS_DATA_TYPES_HPP__

// EOWS
#include "../../geoarray/data_types.hpp"

// STL
#include <memory>
#include <string>
#include <vector>

namespace eows
{
  namespace ogc
  {
    namespace wms
    {

      struct online_resource_t
      {
        std::string xlink_type;
        std::string href;
      };

      struct contact_person_primary_t
      {
        std::string contact_person;
        std::string contact_organization;
      };

      struct contact_address_t
      {
        std::string address_type;
        std::string address;
        std::string city;
        std::string state_or_province;
        std::string post_code;
        std::string country;
      };

      struct contact_information_t
      {
        contact_person_primary_t contact_person_primary;
        std::string contact_position;
        contact_address_t contact_address;
        std::string contact_voice_telephone;
        std::string contact_facsimile_telephone;
        std::string contact_electronic_mail_address;
      };

      struct service_t
      {
        std::string title;
        std::string abstract;
        std::vector<std::string> keyword_list;
        online_resource_t online_resource;
        contact_information_t contact_information;
        uint32_t layer_limit;
        uint32_t max_width;
        uint32_t max_height;
      };

      struct dcp_type_t
      {
        std::string get;
        std::string post;
      };

      struct operation_type_t
      {
        std::vector<std::string> format_list;
        dcp_type_t dcp_type;
      };

      struct request_t
      {
        operation_type_t get_capabilities;
        operation_type_t get_map;
        operation_type_t get_feature_info;
      };

      struct exception_t
      {
        std::vector<std::string> format_list;
      };

      struct bounding_box_t
      {
        eows::geoarray::spatial_extent_t extent;
        std::string crs;
        double res_x;
        double res_y;
      };

//      struct dimension_t
//      {
//        std::string value;
//        std::string name;
//        std::string units;
//        std::string default_value;
//      };

//      struct logo_url_t
//      {
//        std::string format;
//        online_resource_t online_resource;
//        uint32_t width;
//        uint32_t height;
//      };

//      struct attribution_t
//      {
//        std::string title;
//        online_resource_t online_resource;
//        logo_url_t logo_url;
//      };

//      struct authority_url_t
//      {
//        online_resource_t online_resource;
//        std::string nmtoken;
//      };

//      struct identifier_t
//      {
//        std::string value;
//        std::string authority;
//      };

//      struct metadata_url_t
//      {
//        std::string format;
//        online_resource_t online_resource;
//        std::string nmtoken;
//      };

//      struct data_url_t
//      {
//        std::string format;
//        online_resource_t online_resource;
//      };

//      struct feature_list_url_t
//      {
//          std::string format;
//          online_resource_t online_resource;
//      };

//      struct legend_url_t
//      {
//        std::string format;
//        online_resource_t online_resource;
//        uint32_t width;
//        uint32_t height;
//      };

//      struct style_sheet_url_t
//      {
//        std::string format;
//        online_resource_t online_resource;
//      };

//      struct style_url_t
//      {
//        std::string format;
//        online_resource_t online_resource;
//      };

//      struct style_t
//      {
//        std::string name;
//        std::string title;
//        std::string abstract;
//        std::vector<legend_url_t> legend_url_list;
//        style_sheet_url_t style_sheet_url;
//        style_url_t style_url;
//      };

      struct layer_t
      {
        std::string name;
        std::string title;
        std::string abstract;
        std::vector<std::string> keyword_list;
        std::vector<std::string> crs_list;
        eows::geoarray::spatial_extent_t ex_geographic_bounding_box;
        std::vector<bounding_box_t> bounding_box_list;
        //std::vector<dimension_t> dimension_list;
        //attribution_t attribution;
        //std::vector<authority_url_t> authority_url_list;
        //std::vector<identifier_t> identifier_list;
        //std::vector<metadata_url_t> metadata_url_list;
        //std::vector<data_url_t> data_url_list;
        //std::vector<feature_list_url_t> feature_list_url_list;
        //std::vector<style_t> style_list;
        //double min_scale_denominator;
        //double max_scale_denominator;
        std::vector<std::shared_ptr<layer_t> > layer_list;
      };

      struct capability_t
      {
        request_t request;
        exception_t exception;
        layer_t layer;
      };

      struct capabilities_t
      {
        service_t service;
        capability_t capability;
      };

    }
  }
}

#endif // __EOWS_OGC_WMS_DATA_TYPES_HPP__
