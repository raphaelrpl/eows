#include "manager.hpp"
#include "defines.hpp"
#include "exception.hpp"
#include "data_types.hpp"

// EOWS OAuth2
#include "oauth2/data_types.hpp"

// EOWS Core
#include "../core/app_settings.hpp"
#include "../core/utils.hpp"
#include "../core/http_request.hpp"

// STL
#include <string>
#include <vector>
#include <memory>

// RapidJSON
#include <rapidjson/document.h>

// Boost
#include <boost/filesystem/path.hpp>

struct eows::auth::manager::impl
{
  eows::auth::config_t config;
  std::vector<eows::auth::user_t> users;

  std::vector<std::unique_ptr<eows::auth::oauth_client>> clients;
  std::vector<std::unique_ptr<eows::auth::session>> sessions;

  bool loaded = false;
};

eows::auth::manager::~manager()
{
  delete pimpl_;
}

eows::auth::manager&eows::auth::manager::instance()
{
  static manager m;
  return m;
}

const eows::auth::config_t&eows::auth::manager::settings() const
{
  return pimpl_->config;
}

void eows::auth::manager::initialize()
{
  if (!pimpl_->loaded)
  {
    boost::filesystem::path cfg_file(eows::core::app_settings::instance().get_base_dir());

    cfg_file /= EOWS_AUTH_FILE;

    rapidjson::Document doc = eows::core::open_json_file(cfg_file.string());

    if(!doc.IsObject())
      throw eows::parse_error("Auth file is not a object '" EOWS_AUTH_FILE "'.");

    pimpl_->config.oauth2_authorize_uri = eows::core::read_node_as_string(doc, "oauth2_authorize_uri");
    pimpl_->config.oauth2_logout_uri = eows::core::read_node_as_string(doc, "oauth2_logout_uri");
    pimpl_->config.oauth2_info_uri = eows::core::read_node_as_string(doc, "oauth2_info_uri");
    pimpl_->config.oauth2_login_template_path = eows::core::read_node_as_string(doc, "oauth2_login_template_path");
    pimpl_->config.oauth2_message_template_path = eows::core::read_node_as_string(doc, "oauth2_message_template_path");
    pimpl_->config.oauth2_error_template_path = eows::core::read_node_as_string(doc, "oauth2_error_template_path");
    pimpl_->config.use_refresh_token = eows::core::read_node_as_bool(doc, "use_refresh_token");

    rapidjson::Value::ConstMemberIterator it = doc.FindMember("users");
    if (it == doc.MemberEnd())
      throw eows::parse_error("No admin user set");

    if (!it->value.IsArray())
      throw eows::parse_error("Key \"users\" must be an array of objects");

    for(const auto& user_elm: it->value.GetArray())
    {
      user_t u;
      u.username = eows::core::read_node_as_string(user_elm, "username");
      u.password = eows::core::read_node_as_string(user_elm, "password");
      pimpl_->users.push_back(u);
    }

    std::unique_ptr<oauth_client> dummy_client(new oauth_client);
    dummy_client->key = "key_secret";
    std::vector<std::string> dummy_roles;
    dummy_roles.push_back("wcs.all");
    dummy_roles.push_back("wtss.all");
    dummy_roles.push_back("wtsps.all");
    dummy_roles.push_back("global.users");
    dummy_roles.push_back("global.workspace");
    dummy_client->roles = dummy_roles;
    dummy_client->type = "public";
    pimpl_->clients.push_back(std::move(dummy_client));

    pimpl_->loaded = true;
  }
}

eows::auth::oauth_client* eows::auth::manager::find_client(const std::string& client_id) const
{
  auto found = std::find_if(pimpl_->clients.begin(),
                            pimpl_->clients.end(),
                            [&client_id] (const std::unique_ptr<oauth_client>& client)
                            {
                              return client->key == client_id; // TODO: remove it since client.key will be encrypted
                            });
  if (found != pimpl_->clients.end())
    return found->get();
  return nullptr;
}

eows::auth::session*eows::auth::manager::find_session(const std::string& token) const
{
  auto found = std::find_if(pimpl_->sessions.begin(),
                            pimpl_->sessions.end(),
                            [&token] (const std::unique_ptr<session>& client)
                            {
                              return client->token == token;
                            });
  if (found != pimpl_->sessions.end())
    return found->get();
  return nullptr;
}

eows::auth::session*eows::auth::manager::find_session(const eows::core::http_request& request) const
{
  std::string token;

  // Query String
  {
    auto query_string = request.query_string();
    auto it = query_string.find("access_token");
    if (it != query_string.end())
      token = it->second;
  }

  // Body
  {
    auto body = request.data();
    auto it = body.find("access_token");
    if (it != body.end())
      token = it->second;
  }

  return find_session(token);
}

void eows::auth::manager::create_client(const std::string& type, const std::vector<std::string>& redirect_uris, const std::string& application_name, const std::vector<std::string>& roles, std::string& secret)
{
  std::unique_ptr<oauth_client> client(new oauth_client);

  client->id = "unique_id_" + application_name;
//oauth2.client:value:myfNv849Z1GNuPAN
  client->key = "encrypted_" + client->id;
  client->type = type;
  client->application_name = application_name;
  client->roles = roles;
  client->redirect_uris = redirect_uris;
  secret = "secret_used_to_encrypt_" + client->key;

  pimpl_->clients.push_back(std::move(client));
}

eows::auth::manager::manager()
  : pimpl_(new impl)
{

}
