#include "data_types.hpp"
#include "../core/data_types.hpp"
#include "../../../core/utils.hpp"
#include "../manager.hpp"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

eows::ogc::wcs::operations::base_request::base_request(const eows::core::query_string_t& query)
  : request(), version(), service()
{
  const eows::ogc::wcs::core::capabilities_t& capabilities = manager::instance().capabilities();

  eows::core::query_string_t::const_iterator it = query.find("request");

  if (it == query.end() || it->second.empty()) {
    throw eows::ogc::missing_parameter_error("Missing parameter 'request'", "request");
  }
  request = it->second[0];

  it = query.find("service");

  if (it == query.end() || it->second.empty()) {
    throw eows::ogc::missing_parameter_error("Missing parameter 'service'", "service");
  }

  if (eows::core::to_lower(it->second[0]) != "wcs")
    throw eows::ogc::invalid_parameter_error("No service '" + it->second[0] + "'", "InvalidParameterValue");

  service = it->second[0];

  it = query.find("version");

  if (it != query.end() && !it->second.empty())
  {
    if (it->second[0] != capabilities.service.service_type_version)
      throw eows::ogc::not_supported_error("WCS version '"+it->second[0]+
                                           "' is not supported. Try '"+
                                           capabilities.service.service_type_version+"'", "VersionNegotiationFailed");
    version = it->second[0];
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
  boost::split(sliced_coverages, it->second[0], boost::is_any_of(","));
  coverages_id = sliced_coverages;
}

eows::ogc::wcs::operations::get_coverage_request::get_coverage_request(const eows::core::query_string_t& query)
  : base_request(query)
{
  eows::core::query_string_t::const_iterator it = query.find("coverageid");

  if (it == query.end() || it->second.empty())
  {
    throw eows::ogc::missing_parameter_error("Missing parameter 'CoverageID'", "emptyCoverageIdList");
  }
  coverage_id = it->second[0];

  it = query.find("format");

  std::string default_format = "application/gml+xml";
  // Setting default format
//  if (it == query.end())
    format = default_format;
//  else
//  {
//    if (it->second != default_format)
//      throw eows::ogc::invalid_parameter_error("Format '" + it->second + "' not supported", "format");

//    format = it->second;
//  }
  // Process Subsets
}
