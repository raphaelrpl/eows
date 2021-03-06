// EOWS
#include "data_types.hpp"
#include "../core/data_types.hpp"
#include "../manager.hpp"
// EOWS core (to_lower)
#include "../../../core/utils.hpp"

// STL (stringstream parse and find)
#include <sstream>

// Boost
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

eows::ogc::wcs::operations::base_request::base_request(const eows::core::query_string_t& query)
  : request(), version(), service()
{
  const eows::ogc::wcs::core::capabilities_t& capabilities = manager::instance().capabilities();

  eows::core::query_string_t::const_iterator it = query.find("request");

  if (it == query.end()) {
    throw eows::ogc::missing_parameter_error("Missing parameter 'request'", "request");
  }
  request = it->second;

  it = query.find("service");

  if (it == query.end()) {
    throw eows::ogc::missing_parameter_error("Missing parameter 'service'", "service");
  }

  if (eows::core::to_lower(it->second) != "wcs")
    throw eows::ogc::invalid_parameter_error("No service '" + it->second + "'", "InvalidParameterValue");

  service = it->second;

  it = query.find("version");

  if (it != query.end())
  {
    if (it->second != capabilities.service.service_type_version)
      throw eows::ogc::not_supported_error("WCS version '"+it->second+
                                           "' is not supported. Try '"+
                                           capabilities.service.service_type_version+"'", "VersionNegotiationFailed");
    version = it->second;
  }
}

eows::ogc::wcs::operations::base_request::~base_request()
{
}

eows::ogc::wcs::operations::get_capabilities_request::get_capabilities_request(const eows::core::query_string_t& query)
  : base_request(query)
{

}

eows::ogc::wcs::operations::describe_coverage_request::describe_coverage_request(const eows::core::query_string_t& query)
  : base_request(query)
{
  eows::core::query_string_t::const_iterator it = query.find("coverageid");

  if (it == query.end())
  {
    throw eows::ogc::missing_parameter_error("Missing parameter 'CoverageID'", "emptyCoverageIdList");
  }

  std::vector<std::string> sliced_coverages;

  boost::split(sliced_coverages, it->second, boost::is_any_of(","));
  coverages_id = sliced_coverages;
}

void validate_subset(const std::stringstream& ss)
{
  if (!ss.good())
    throw eows::ogc::invalid_parameter_error("Invalid subset axis", "InvalidAxisLabel");
}

eows::ogc::wcs::operations::get_coverage_request::get_coverage_request(const eows::core::query_string_t& query)
  : base_request(query)
{
  eows::core::query_string_t::const_iterator it = query.find("coverageid");

  if (it == query.end())
    throw eows::ogc::missing_parameter_error("Missing parameter 'CoverageID'", "emptyCoverageIdList");

  coverage_id = it->second;

  it = query.find("format");

  // Setting default format
  if (it == query.end())
    format = eows::core::APPLICATION_XML;
  else
    format = eows::core::from_string(it->second);

  // InputCRS
  it = query.find("inputcrs");
  if (it != query.end())
  {
    std::stringstream stream;
    stream << it->second;
    stream >> input_crs;
  }
  else
    input_crs = 4326;

  it = query.find("outputcrs");
  if (it != query.end())
  {
    std::stringstream stream;
    stream << it->second;
    stream >> output_crs;
  }

  // Process Subsets
  digest_subset(query);

  it = query.find("rangesubset");

  if (it != query.end())
  {
    boost::split(range_subset.attributes, it->second, boost::is_any_of(","));
    range_subset.raw = it->second;
  }
}

void eows::ogc::wcs::operations::get_coverage_request::digest_subset(const eows::core::query_string_t& query)
{
  eows::core::query_string_t::const_iterator begin_it  = query.lower_bound("subset");

  if (begin_it != query.end())
  {
    eows::core::query_string_t::const_iterator end_it = query.upper_bound("subset");

    std::stringstream ss;

    while(begin_it != end_it)
    {
      const std::string subset_str = begin_it->second;

      eows::ogc::wcs::core::subset_t dimension;

      // TODO: Should use regex? (?<axis>[xyt]+)\((?<min>\d+),(?<max>\d+)\)
      ss << subset_str;
      validate_subset(ss);

      // Retrieve axis name
      char c;
      while(ss >> c && (c != '(' && c != ','))
      {
        dimension.name += c;
        validate_subset(ss);
      }

      // Seeking for duplicated keys
      auto found = std::find_if(subsets.begin(), subsets.end(), [&dimension](const eows::ogc::wcs::core::subset_t& elm) {
        return elm.name == dimension.name;
      });

      if (found != subsets.end())
        throw eows::ogc::wcs::invalid_axis_error("Duplicated axis " + dimension.name);

      // Checking if client informed SRS in axis
      if (c == ',')
      {
        while(ss >> c && c != '(')
        {
          dimension.srid += c;
          validate_subset(ss);
        }
      }

      while(ss >> c && (c != ',' && c != ')'))
      {
        dimension.min += c;
        validate_subset(ss);
      }

      if (c == ')')
        dimension.max = eows::ogc::wcs::core::subset_t::no_value;
      else if (c == ',')
      {
        validate_subset(ss);
        // Retrieve axis max
        while(ss >> c && c != ')')
        {
          dimension.max += c;
          validate_subset(ss);
        }
      }
      else
        throw eows::ogc::wcs::invalid_axis_error("Invalid axis while parsing " + dimension.name);

      subsets.push_back(dimension);

      // Cleaning stream
      ss.str("");
      ss.clear();

      // Incrementing iterator
      ++begin_it;
    }
  }  // end if it(subset) != end()
}
