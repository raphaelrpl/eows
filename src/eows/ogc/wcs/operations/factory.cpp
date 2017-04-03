// EOWS WCS
#include "factory.hpp"
#include "../core/operation.hpp"

#include "../exception.hpp"
// WCS Operation Data Types
#include "data_types.hpp"
#include "get_capabilities.hpp"

const eows::ogc::wcs::core::operation* eows::ogc::wcs::operations::build_operation(const eows::core::query_string_t& query)
{
  eows::core::query_string_t::const_iterator request_it = query.find("request");

  if (request_it == query.end()) {
    throw eows::ogc::missing_parameter_error("Missing parameter 'request'");
  }

  eows::core::query_string_t::const_iterator it = query.find("version");

  if (it == query.end()) {
    throw eows::ogc::missing_parameter_error("Missing parameter 'version'");
  }

  it = query.find("service");

  if (it == query.end()) {
    throw eows::ogc::missing_parameter_error("Missing parameter 'service'");
  }

  // Checking which operation should build
  // TODO: Put everything to lowercase-style
  if (request_it->second == "GetCapabilities")
  {
    GetCapabilitiesRequest capabilities_request;
    return new get_capabilities();
  }
  else if (request_it->second == "DescribeCoverage")
  {
    throw eows::ogc::not_implemented_error("Missing parameter 'VERSION'");
  }
  else //if (request_it->second == "GetCoverage")
  {
    throw eows::ogc::not_implemented_error("Missing parameter 'VERSION'");
  }
}
