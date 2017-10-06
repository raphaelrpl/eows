#include "token.hpp"
#include "exception.hpp"

// STL
#include <memory>

// JWT
#include <jwtxx/jwt.h>

// Defines Global Algorithm for Token handling
static JWTXX::Algorithm algorithm = JWTXX::Algorithm::HS256;

struct eows::auth::token_t::impl
{
  std::string token;
  std::unique_ptr<JWTXX::JWT> jwt;

  void initialize(const eows::auth::token_t::metadata_t& information);
};

inline void eows::auth::token_t::impl::initialize(const eows::auth::token_t::metadata_t& information)
{
  jwt.reset(new JWTXX::JWT(algorithm, information));
  token = jwt->token("secret");
}

eows::auth::token_t::token_t(const std::string& token)
  : pimpl_(new impl)
{
  pimpl_->token = token;

  try
  {
    pimpl_->jwt.reset(new JWTXX::JWT(pimpl_->token,
                                     JWTXX::Key(algorithm, "secret")));
  }
  catch(const JWTXX::JWT::ParseError& e)
  {
    throw invalid_request_error("No token provided");
  }
  catch(const JWTXX::JWT::Error& e)
  {
    throw invalid_token_error(e.what());
  }
}

eows::auth::token_t::token_t(const metadata_t& information)
  : pimpl_(new impl)
{
  pimpl_->initialize(information);
}

eows::auth::token_t::~token_t()
{
  delete pimpl_;
}

const std::string eows::auth::token_t::claim(const std::string& key) const
{
  return pimpl_->jwt->claim(key);
}

const eows::auth::token_t::metadata_t eows::auth::token_t::claim() const
{
  if (!expired())
    return pimpl_->jwt->claims();

  return eows::auth::token_t::metadata_t();
}

const std::string eows::auth::token_t::token() const
{
  return pimpl_->token;
}

bool eows::auth::token_t::expired() const
{
  try
  {
    JWTXX::JWT j(pimpl_->token, JWTXX::Key(algorithm, "secret"));

    auto active = j.claim("active");

    if (active.empty())
      return false;

    return active != "true";
  }
  catch(...)
  {
    return true;
  }
}

const std::string eows::auth::token_t::to_json() const
{
  std::string json;

  // If token is already expired, just reply active as false.
  if (expired())
    json = "{\"active\": false}";
  else
  {
    auto metadata = claim();

    json += ",\"active\": true";

    for(const auto& it: metadata)
      json += ",\"" + it.first + "\": \"" + it.second + "\"";

    if (json.empty())
      json = "{}";
    else
    {
      json[0] = '{';
      json += "}";
    }
  }

  return json;
}
