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
  \file eows/geoarray/data_types.hpp

  \brief Type declaration and definition for GeoArrays.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __EOWS_GEOARRAY_DATA_TYPES_HPP__
#define __EOWS_GEOARRAY_DATA_TYPES_HPP__

// EOWS
#include "exception.hpp"

// STL
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iterator>
#include <map>
#include <string>
#include <vector>

// Boost
#include <boost/format.hpp>

namespace eows
{
  namespace geoarray
  {

    //! The spatial resolution of array cells.
    struct spatial_resolution_t
    {
      double x;
      double y;
    };

    //! The range of valid values for a given attribute of an array.
    struct numeric_range_t
    {
      double min_val;
      double max_val;
    };

    //! The spatial extent of an array.
    struct spatial_extent_t
    {
      double xmin;
      double ymin;
      double xmax;
      double ymax;

      bool intersects(const double& x, const double& y) const
      {
        if(x < xmin)
          return false;

        if(x > xmax)
          return false;

        if(y < ymin)
          return false;

        if(y > ymax)
          return false;

        return true;
      }
    };

    //! The list of data types for array attributes.
    struct datatype_t
    {
      enum
      {
        int8_dt,
        uint8_dt,
        int16_dt,
        uint16_dt,
        int32_dt,
        uint32_dt,
        int64_dt,
        uint64_dt,
        float_dt,
        double_dt,
        unknown
      };

      //! Convert a data type id to its string description.
      static inline std::string to_string(int dt)
      {
        switch(dt)
        {
          case int8_dt: return "int8";
          case uint8_dt: return "uint8";
          case int16_dt: return "int16";
          case uint16_dt: return "uint16";
          case int32_dt: return "int32";
          case uint32_dt: return "uint32";
          case int64_dt: return "int64";
          case uint64_dt: return "uint64";
          case float_dt: return "float";
          case double_dt: return "double";
          default: return "unknown";
        }
      }

      static inline int bytes(int dt)
      {
        switch(dt)
        {
          case eows::geoarray::datatype_t::int8_dt:
            return sizeof(char) * 8;
          case eows::geoarray::datatype_t::uint8_dt:
            return sizeof(unsigned char);
          case eows::geoarray::datatype_t::int16_dt:
            return sizeof(int16_t);
          case eows::geoarray::datatype_t::int32_dt:
            return sizeof(int32_t);
          case eows::geoarray::datatype_t::uint16_dt:
            return sizeof(uint16_t);
          case eows::geoarray::datatype_t::uint32_dt:
            return sizeof(uint32_t);
          case eows::geoarray::datatype_t::int64_dt:
            return sizeof(int64_t);
          case eows::geoarray::datatype_t::uint64_dt:
            return sizeof(uint64_t);
          case eows::geoarray::datatype_t::float_dt:
            return sizeof(float);
          case eows::geoarray::datatype_t::double_dt:
            return sizeof(double);
          default:
            return 0;
        }
      }

      //! Convert a data type in its string description to its numeric code.
      static inline int from_string(const std::string& data_type_name)
      {
        if(data_type_name == "int8")
          return int8_dt;
        else if(data_type_name == "uint8")
          return uint8_dt;
        else if(data_type_name == "int16")
          return int16_dt;
        else if(data_type_name == "uint16")
          return uint16_dt;
        else if(data_type_name == "int32")
          return int32_dt;
        else if(data_type_name == "uint32")
          return uint32_dt;
        else if(data_type_name == "int64")
          return int64_dt;
        else if(data_type_name == "uint64")
          return uint64_dt;
        else if(data_type_name == "float")
          return float_dt;
        else if(data_type_name == "double")
          return double_dt;
        else
          return unknown;
      }
    };

    //! The list of units.
    struct unit_t
    {
      enum
      {
        day,
        month,
        year,
        unknown
      };

      //! Convert a unit id to its string description.
      static inline std::string to_string(int u)
      {
        switch(u)
        {
          case day: return "day";
          case month: return "month";
          case year: return "year";
          default: return "unknown";
        }
      }

      //! Convert a unit type in its string description to its numeric code.
      static inline int from_string(const std::string& unit_name)
      {
        if(unit_name == "day")
          return day;
        else if(unit_name == "month")
          return month;
        else if(unit_name == "year")
          return year;
        else
          return unknown;
      }
    };

    //! Dimension description of an array.
    struct dimension_t
    {
      std::string name;
      std::string alias;
      int64_t min_idx;
      int64_t max_idx;
      
      /*!
        \exception std::invalid_argument If an invalid range, such as min > max, is informed.
       */
      explicit dimension_t(const int64_t min = 0, const int64_t max = 0)
        : min_idx(min), max_idx(max)
      {
        if(min_idx > max_idx)
        {
          boost::format err_msg("Dimension range is invalid: [%1%, %2%].");

          throw std::invalid_argument((err_msg % min_idx % max_idx).str());
        }
      }
      
      bool is_in_range(int64_t pos) const
      {
        return (pos >= min_idx && pos <= max_idx);
      }
      
      std::size_t size() const
      {
        return static_cast<std::size_t>(max_idx - min_idx) + 1;
      }
    };
    
    struct dimensions_t
    {
      dimension_t x;
      dimension_t y;
      dimension_t t;

      bool is_range(int64_t col, int64_t row) const
      {
        return x.is_in_range(col) && y.is_in_range(row);
      }
    };

    //!A class for handling the timeline of a geo-array.
    class timeline_t
    {
      public:

        timeline_t() = default;

        timeline_t(std::vector<std::string> time_points, const dimension_t& dim);

        ~timeline_t() = default;

//        /*!
//          \exception std::invalid_argument If temporal dimension is not compatible with time-points.
//         */
//        void set_temporal_dim(dimension_t& dim)
//        {
//          if(dim.size() != time_points_.size())
//          {
//            boost::format err_msg("Temporal dimension [%1%, %2%] is out-of range with timeline of size %3%.");

//            throw std::invalid_argument((err_msg % dim.min_idx % dim.max_idx % time_points_.size()).str());
//          }

//          temporal_dim_ = dim;
//        }

//        const dimension_t& get_temporal_dim() const
//        {
//          return temporal_dim_;
//        }

        //! Returns the timeline.
        const std::vector<std::string>& time_points() const
        {
          return time_points_;
        }

        /*!
          \brief Retrieve the time point in position pos.

          \param pos A number in the range [0:n).

          \exception std::out_of_range If pos is invalid.
         */
        const std::string& get(std::size_t pos) const
        {
          if(pos >= time_points_.size())
          {
            boost::format err_msg("Timeline position '%1%' is out of bounds [0, %2%).");

            throw std::out_of_range((err_msg % pos % time_points_.size()).str());
          }

          return time_points_[pos];
        }

        /*!
          \brief Returns the time index in the temporal dimension.

          \param time_point A number in the range [0:n).

          \return A number in the range of the temporal dimension [min_idx:max_idx].

          \exception std::out_of_range If time point doesn't exist.
         */
        std::size_t index(const std::string& time_point) const
        {
          return pos(time_point) + temporal_dim_.min_idx;
        }

        /*!
          \brief Returns the time position in the timeline.
          
          \return A number in the range [0:n).

          \exception std::out_of_range If time point doesn't exist.
         */
        std::size_t pos(const std::string& time_point) const
        {
          std::map<std::string, std::size_t>::const_iterator it = time_point_idx_.find(time_point);

          if(it == time_point_idx_.end())
          {
            boost::format err_msg("Could not find time point: '%1%'.");

            throw std::out_of_range((err_msg % time_point).str());
          }

          return it->second;
        }

        /*!
          \brief Return the time position in the timeline for a given time index.
          
          \param time_idx A number in the range of the temporal dimension [min_idx:max_idx].

          \return A number in the range [0:n).

          \exception std::out_of_range If time point doesn't exist.
         */
        std::size_t pos(std::size_t time_idx) const
        {
          if(!temporal_dim_.is_in_range(time_idx))
          {
            boost::format err_msg("Invalid time index: '%1%'.");

            throw std::out_of_range((err_msg % time_idx).str());
          }

          return time_idx - temporal_dim_.min_idx;
        }
      
        /*!
          \brief Find an interval based on two dates.
          
          The dates doesn't have to be exactly.
          The output range will follow these rules:
          - The first index will point to a timepoint equal or greater than begin date.
          - The second index will point to a timepoint equal or less than end date.
          
          \return An interval in the range of the temporal dimension [min_idx:max_idx].
          
          \exception std::out_of_range If time interval is not valid.

          \exception std::runtime_error If internal time interval or time index is empty.
         */
        std::pair<std::size_t, std::size_t>
        find_interval(std::string begin, std::string end) const
        {
          if(time_points_.empty() || time_point_idx_.empty())
            throw std::runtime_error("Empty timeline.");

          if(begin.empty())
            begin = time_points().front();

          if(end.empty())
            end = time_points().back();

          if(begin > end)
          {
            boost::format err_msg("Invalid time interval: [%1%, %2%].");
            throw std::out_of_range((err_msg % begin % end).str());
          }

          std::pair<std::size_t, std::size_t> interval;

          {
            std::map<std::string, std::size_t>::const_iterator it = time_point_idx_.find(begin);

            if(it == time_point_idx_.end())
            {
              std::vector<std::string>::const_iterator it = std::upper_bound(time_points_.begin(), time_points_.end(), begin);

              if(it == time_points_.end())
              {
                boost::format err_msg("Could not find the beginning timepoint for the interval: [%1%, %2%].");
                throw std::out_of_range((err_msg % begin % end).str());
              }

              interval.first = std::distance(time_points_.begin(), it) + temporal_dim_.min_idx;
            }
            else
            {
             interval.first = it->second + temporal_dim_.min_idx;
            }
          }

          {
            std::map<std::string, std::size_t>::const_iterator it = time_point_idx_.find(end);

            if(it == time_point_idx_.end())
            {
              std::vector<std::string>::const_iterator it = std::lower_bound(time_points_.begin(), time_points_.end(), end);

              if(it == time_points_.begin())
              {
                boost::format err_msg("Could not find the end timepoint for the interval: [%1%, %2%].");
                throw std::out_of_range((err_msg % begin % end).str());
              }

              --it;

              interval.second = std::distance(time_points_.begin(), it) + temporal_dim_.min_idx;
            }
            else
            {
             interval.second = it->second + temporal_dim_.min_idx;
            }
          }
          
          if(interval.first > interval.second)
          {
            boost::format err_msg("Time interval invalid: [%1%, %2%].");

            throw std::out_of_range((err_msg % begin % end).str());
          }
          
          return interval;
        }
      
      private:

        std::vector<std::string> time_points_;
        std::map<std::string, std::size_t> time_point_idx_;
        dimension_t temporal_dim_;
    };

    //! Base metadata of an array attribute.
    struct attribute_t
    {
      std::string name;
      std::string description;
      numeric_range_t valid_range;
      double scale_factor;
      double missing_value;
      int datatype;
    };

    struct internal_metadata_t
    {
      spatial_extent_t spatial_extent;
      spatial_resolution_t spatial_resolution;
      std::size_t srid;
    };

    //! Base metadata of an array.
    struct geoarray_t
    {
      std::string name;
      std::string cluster_id;
      std::string description;
      std::string detail;
      std::vector<attribute_t> attributes;
      dimensions_t dimensions;
      spatial_extent_t spatial_extent;
      spatial_resolution_t spatial_resolution;
      std::size_t srid;
      timeline_t timeline;
      internal_metadata_t i_meta;
    };

    struct grid
    {
      const geoarray_t* geo_array;
      double half_res_x;
      double half_res_y;

      grid(const geoarray_t* garray)
        : geo_array(garray),
          half_res_x(garray->i_meta.spatial_resolution.x / 2.0),
          half_res_y(garray->i_meta.spatial_resolution.y / 2.0)
      {
      }

      int64_t col(const double& x) const
      {
        double dcol = (x - geo_array->i_meta.spatial_extent.xmin) / geo_array->i_meta.spatial_resolution.x;

        return static_cast<int64_t>(dcol) + geo_array->dimensions.x.min_idx;
      }

      int64_t row(const double& y) const
      {
        double drow = (y - geo_array->i_meta.spatial_extent.ymin) / geo_array->i_meta.spatial_resolution.y;

        return static_cast<int64_t>(drow) + geo_array->dimensions.y.min_idx;
      }

      double x(int64_t col) const
      {
        return (static_cast<double>(col) * geo_array->i_meta.spatial_resolution.x) + half_res_x + geo_array->i_meta.spatial_extent.xmin;
      }

      double y(int64_t row) const
      {
        return geo_array->i_meta.spatial_extent.ymax - (static_cast<double>(row) * geo_array->i_meta.spatial_resolution.y) - half_res_y;
      }
    };
    
  } // end namespace geoarray
}   // end namespace eows

#endif // __EOWS_GEOARRAY_DATA_TYPES_HPP__
