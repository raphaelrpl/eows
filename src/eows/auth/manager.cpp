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
  \file eows/auth/manager.cpp

  \brief Implementation of manager for Auth configurations.

  \author Raphael Willian da Costa
 */

#include "manager.hpp"
#include "defines.hpp"
#include "exception.hpp"
#include "data_types.hpp"
#include "utils.hpp"

// EOWS OAuth2
#include "oauth2/data_types.hpp"
#include "oauth2/generator.hpp"

// EOWS Core
#include "../core/app_settings.hpp"
#include "../core/utils.hpp"
#include "../core/http_request.hpp"

// STL
#include <string>
#include <vector>
#include <fstream>

// RapidJSON
#include <rapidjson/document.h>

// Boost
#include <boost/filesystem/path.hpp>

struct eows::auth::manager::impl
{
  eows::auth::config_t config;
  std::vector<std::unique_ptr<eows::auth::user_t>> users;
  std::vector<std::unique_ptr<eows::auth::oauth_client>> clients;
  std::vector<std::unique_ptr<eows::auth::session>> sessions;
  std::vector<std::unique_ptr<eows::auth::oauth_code>> codes;

  std::string authorize_template;
  std::string login_template;
  std::string error_template;

  int client_id_length = 16;

  bool loaded = false;

  /*!
   * \brief Open file contents
   * \param file_path Path to the file
   * \return File contents
   */
  std::string open_file(const std::string& file_path);

  //! Loads every OAuth2 Template in memory
  void load_templates();
};

inline std::string eows::auth::manager::impl::open_file(const std::string& file_path)
{
  boost::filesystem::path template_file(eows::core::app_settings::instance().get_base_dir());

  template_file /= file_path;
  std::ifstream file(template_file.string(), std::ifstream::in);

  return std::string(std::istreambuf_iterator<char>(file),
                     std::istreambuf_iterator<char>());
}

inline void eows::auth::manager::impl::load_templates()
{
  // TODO: Throw error if file not found
  authorize_template = open_file(config.oauth2_message_template_path);
  login_template = open_file(config.oauth2_login_template_path);
  error_template = open_file(config.oauth2_error_template_path);
}

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

const std::string&eows::auth::manager::login_template() const
{
  return pimpl_->login_template;
}

const std::string&eows::auth::manager::authorize_template() const
{
  return pimpl_->authorize_template;
}

const std::string&eows::auth::manager::error_template() const
{
  return pimpl_->error_template;
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
    pimpl_->config.oauth2_token_uri = eows::core::read_node_as_string(doc, "oauth2_token_uri");
    pimpl_->config.oauth2_login_template_path = eows::core::read_node_as_string(doc, "oauth2_login_template_path");
    pimpl_->config.oauth2_message_template_path = eows::core::read_node_as_string(doc, "oauth2_message_template_path");
    pimpl_->config.oauth2_error_template_path = eows::core::read_node_as_string(doc, "oauth2_error_template_path");
    pimpl_->config.use_refresh_token = eows::core::read_node_as_bool(doc, "use_refresh_token");
    pimpl_->config.session_expiration = eows::core::read_node_as_int64(doc, "session_expiration");
    pimpl_->config.oauth2_code_expiration = eows::core::read_node_as_int64(doc, "oauth2_code_expiration");
    // Loading HTML Templates
    pimpl_->load_templates();

    rapidjson::Value::ConstMemberIterator it = doc.FindMember("users");
    if (it == doc.MemberEnd())
      throw eows::parse_error("No admin user set");

    if (!it->value.IsArray())
      throw eows::parse_error("Key \"users\" must be an array of objects");

    for(const auto& user_elm: it->value.GetArray())
    {
      std::unique_ptr<user_t> u(new user_t);
      u->full_name = eows::core::read_node_as_string(user_elm, "full_name");
      u->username = eows::core::read_node_as_string(user_elm, "username");
      u->password = eows::core::read_node_as_string(user_elm, "password");
      pimpl_->users.push_back(std::move(u));
    }
    std::vector<std::string> dummy_roles;
    dummy_roles.push_back("user.email");
    dummy_roles.push_back("wcs");
    std::vector<std::string> uris;
    uris.push_back("http://127.0.0.1:7654/echo");
    uris.push_back("http://localhost:7654/echo");
    uris.push_back("http://localhost:7654/oauth2/authorize");
    uris.push_back("http://127.0.0.1:7654/oauth2/authorize");
    uris.push_back("http://127.0.0.1:5000/login/authorized");
    uris.push_back("http://127.0.0.1:5000/callback");

    std::string secret;
    create_client("public", uris, "Teste APP", dummy_roles, secret);

    pimpl_->loaded = true;
  }
}

eows::auth::oauth_client* eows::auth::manager::find_client(const std::string& client_id) const
{
  auto found = std::find_if(pimpl_->clients.begin(),
                            pimpl_->clients.end(),
                            [&client_id] (const std::unique_ptr<oauth_client>& client)
                            {
                              return client->id == client_id; // TODO: remove it since client.key will be encrypted
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

eows::auth::session*eows::auth::manager::find_session(const eows::core::http_request& request, core::http_response& response) const
{
  std::string token;

  // Cookies
  auto cookies = request.cookies();

  auto cookie_it = cookies.find(session_label);

  if (cookie_it != cookies.end())
    token = cookie_it->second;
  else
  {
    // Query String
    auto query_string = request.query_string();
    auto it = query_string.find(session_label);
    if (it != query_string.end())
      token = it->second;
    else
    // Body
    {
      auto body = request.data();
      auto it = body.find(session_label);
      if (it != body.end())
        token = it->second;
      else
      {
        /* Throw Error */
      }
    }
  }

  auto s = find_session(token);

  if (s == nullptr)
  {
    // create session
    std::unique_ptr<session> session_ptr(new session);
    session_ptr->token = generate(12);
    session_ptr->update_time = std::time(0);

    s = session_ptr.get();
    pimpl_->sessions.push_back(std::move(session_ptr));
  }
  response.add_header(eows::core::http_response::SET_COOKIE, session_label+"="+s->token + "; HttpOnly; Path=/");
  return s;
}

eows::auth::user_t*eows::auth::manager::find_user(const std::string& username) const
{
  for(const auto& user: pimpl_->users)
    if (user->username == username)
      return user.get();
  return nullptr;
}

eows::auth::oauth_code* eows::auth::manager::find_code(const std::string& code)
{
  for(const auto& c: pimpl_->codes)
    if (c->id == code)
      return c.get();
  return nullptr;
}

void eows::auth::manager::create_client(const std::string& type, const std::vector<std::string>& redirect_uris,
                                        const std::string& application_name, const std::vector<std::string>& roles,
                                        std::string& secret)
{
  std::unique_ptr<oauth_client> client(new oauth_client);

  // Generating Secret based 12 byte length.
  secret = "some_secret";//generate(12);

  client->id = "some_id";//generate(pimpl_->client_id_length);
  client->key = encrypt(client->id, secret);
  client->type = type;
  client->application_name = application_name;
  client->roles = roles;
  client->redirect_uris = redirect_uris;

  std::cout << "Client ID: " << client->id << std::endl;
  std::cout << "Client KEY: " << client->key << std::endl;
  std::cout << "Client Secret: " << secret << std::endl;

  pimpl_->clients.push_back(std::move(client));
}

void eows::auth::manager::create_code(std::unique_ptr<eows::auth::oauth_code> code)
{
  pimpl_->codes.push_back(std::move(code));
}

void eows::auth::manager::remove_session(eows::auth::session* s)
{
  if (s != nullptr)
  {
    std::remove_if(pimpl_->sessions.begin(),
                   pimpl_->sessions.end(),
                   [s](const std::unique_ptr<eows::auth::session>& session_ptr)
                   {
                     return session_ptr->token == s->token;
                   });
  }
}

eows::auth::manager::manager()
  : pimpl_(new impl)
{
}
