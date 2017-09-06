#include "utils.hpp"
#include "data_types.hpp"

// STL
#include <sstream>

// Boost
#include <boost/format.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

void eows::auth::access_denied(eows::auth::oauth_parameters& parameters)
{
  parameters.error = "access_denied";
  parameters.error_description = "Access Denied";
}

void eows::auth::unauthorized(eows::auth::oauth_parameters& parameters)
{
  parameters.error = "unauthorized_client";
  parameters.error_description = "Unauthorized Client";
}

void eows::auth::unsupported(eows::auth::oauth_parameters& parameters)
{
  parameters.error = "unsupported";
  parameters.error_description = "Unsupported Response Type";
}

void eows::auth::invalid_request(eows::auth::oauth_parameters& parameters, const std::string& info)
{
  parameters.error = "invalid_request";
  parameters.error_description = "Invalid Request. " + info;
}

std::string eows::auth::generate_token()
{
  boost::uuids::uuid u = boost::uuids::random_generator()();
  std::stringstream s;
  s << u;
  std::string mys = s.str();
  mys.erase(8,1); mys.erase(12,1);
  mys.erase(16,1); mys.erase(20,1);
  return mys;
}
