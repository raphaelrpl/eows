#include "utils.hpp"

// STL
#include <sstream>

// Boost
#include <boost/format.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

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
