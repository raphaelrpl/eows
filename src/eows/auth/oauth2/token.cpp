#include "token.hpp"
#include "exception.hpp"

// STL
#include <unordered_map>
#include <memory>

// JWT
#include <jwtxx/jwt.h>

static JWTXX::Algorithm algorithm = JWTXX::Algorithm::HS256;

struct eows::auth::token_t::impl
{
  std::string token;
  std::unique_ptr<JWTXX::JWT> jwt;
  std::unordered_map<std::string, std::string> values;
  bool initialized = false;
};

eows::auth::token_t::token_t(const std::string& token)
  : pimpl_(new impl)
{
  pimpl_->token = token;

  try
  {
    pimpl_->jwt.reset(new JWTXX::JWT(pimpl_->token,
                                     JWTXX::Key(algorithm, "secret")));
  }
  catch(const JWTXX::JWT::Error& error)
  {
    // TODO: check https://www.oauth.com/oauth2-servers/token-introspection-endpoint/#error
    throw invalid_client_error("The client authentication was invalid");
  }
}

eows::auth::token_t::token_t(const metadata_t& information)
  : pimpl_(new impl)
{
  pimpl_->jwt.reset(new JWTXX::JWT(algorithm, information));
  pimpl_->token = pimpl_->jwt->token("secret");
}

eows::auth::token_t::~token_t()
{
  delete pimpl_;
}

void eows::auth::token_t::digest()
{
  pimpl_->initialized = true;
  pimpl_->jwt.reset(new JWTXX::JWT(JWTXX::Algorithm::RS512, pimpl_->values));
  pimpl_->token = pimpl_->jwt->token("secret");
}

void eows::auth::token_t::attach(const std::string& key, const std::string& value)
{
  if (!pimpl_->initialized)
    pimpl_->values.insert(std::make_pair(key, value));
}

const std::string eows::auth::token_t::claim(const std::string& key)
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
