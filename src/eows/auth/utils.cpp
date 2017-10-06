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
  \file eows/auth/utils.hpp

  \brief Implementation of Authentication utility helpers

  \author Raphael Willian da Costa
 */

#include "utils.hpp"
#include "manager.hpp"
#include "routes.hpp"
#include "data_types.hpp"
#include "exception.hpp"

// EOWS OAuth2
#include "oauth2/token.hpp"
#include "oauth2/exception.hpp"
#include "oauth2/data_types.hpp"

// EOWS Core
#include "../core/logger.hpp"
#include "../core/service_operations_manager.hpp"
#include "../core/utils.hpp"
#include "../core/http_request.hpp"

// STL
#include <cstring>
#include <memory>

// OpenSSL
#include <openssl/aes.h>
#include <openssl/rand.h>

void register_routes()
{
  const auto config = eows::auth::manager::instance().settings();

  eows::core::service_operations_manager::instance().insert(
        "/oauth2/" + config.oauth2_authorize_uri,
        std::unique_ptr<eows::auth::oauth2_authorize>(new eows::auth::oauth2_authorize)
        );

  eows::core::service_operations_manager::instance().insert(
        "/oauth2/" + config.oauth2_info_uri,
        std::unique_ptr<eows::auth::oauth2_info>(new eows::auth::oauth2_info)
        );

  eows::core::service_operations_manager::instance().insert(
        "/oauth2/" + config.oauth2_logout_uri,
        std::unique_ptr<eows::auth::oauth2_logout>(new eows::auth::oauth2_logout)
        );

  eows::core::service_operations_manager::instance().insert(
        "/oauth2/" + config.oauth2_token_uri,
        std::unique_ptr<eows::auth::oauth2_token_handler>(new eows::auth::oauth2_token_handler)
        );

  eows::core::service_operations_manager::instance().insert(
        "/login",
        std::unique_ptr<eows::auth::oauth2_login_handler>(new eows::auth::oauth2_login_handler));

  eows::core::service_operations_manager::instance().insert(
        "/oauth2/example",
        std::unique_ptr<eows::auth::oauth2_example>(new eows::auth::oauth2_example));

  eows::core::service_operations_manager::instance().insert(
        "/oauth2/example2",
        std::unique_ptr<eows::auth::oauth2_example2>(new eows::auth::oauth2_example2));
}

void eows::auth::initialize()
{
  EOWS_LOG_INFO("Initializing EOWS Authentication module...");

  manager::instance().initialize();

  // Prepare OAuth2 Routes
  register_routes();

  EOWS_LOG_INFO("EOWS Authentication module loaded.");
}

void eows::auth::replace(std::string& tpl, const std::multimap<std::string, std::string>& values, const std::string& prefix, const std::string& postfix)
{
  std::string tag;
  std::string v;
  for (const auto& it : values)
  {
    tag.assign(prefix + " " + it.first + " " + postfix);
    v = it.second;
    size_t pos = 0;
    while ((pos = tpl.find(tag, pos)) != std::string::npos)
    {
      tpl.replace(pos, tag.size(), v);
      pos += v.size();
    }
  }
}

std::string eows::auth::encrypt(const std::string& text, std::string password)
{
  password += "qwertyuiopasdfghjklzxcvbnmqwertyqwertyuiopasdfghjklzxcvbnmqwerty";

  const int text_size = text.size() + 1;
  std::unique_ptr<unsigned char> text_to_encrypt(new unsigned char[text_size]);
  std::memcpy(text_to_encrypt.get(), text.c_str(), text.size() + 1);

  const int key_size = password.size() + 1;
  std::unique_ptr<unsigned char> key(new unsigned char[key_size]);

  std::memcpy(key.get(), password.c_str(), password.size() + 1);
  unsigned char enc_out[256];
  AES_KEY enc_key;
  AES_set_encrypt_key(key.get(), 256, &enc_key);
  AES_encrypt(text_to_encrypt.get(), enc_out, &enc_key);

  return std::string(reinterpret_cast<const char*>(enc_out),256);
}

std::string eows::auth::decrypt(const std::string& text, std::string password)
{
  password += "qwertyuiopasdfghjklzxcvbnmqwertyqwertyuiopasdfghjklzxcvbnmqwerty";

  const int key_size = password.size() + 1;
  std::unique_ptr<unsigned char> key(new unsigned char[key_size]);
  std::memcpy(key.get(), password.c_str(), password.size()+1);

  unsigned char dec_out[256];
  AES_KEY dec_key;
  AES_set_decrypt_key(key.get(), 256, &dec_key);

  unsigned char crypted_text[256];
  memcpy(crypted_text, text.c_str(), 256);
  AES_decrypt(crypted_text, dec_out, &dec_key);

  return std::string(reinterpret_cast<const char*>(dec_out));
}

static const std::string default_chars = "abcdefghijklmnaoqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

std::string random_string(size_t len = 15, std::string const &allowed_chars = default_chars)
{
  std::mt19937_64 gen { std::random_device()() };

  std::uniform_int_distribution<size_t> dist { 0, allowed_chars.length()-1 };

  std::string ret;

  std::generate_n(std::back_inserter(ret), len, [&] { return allowed_chars[dist(gen)]; });
  return ret;
}

std::string eows::auth::generate(const int& length)
{
  return random_string(length);
}

bool eows::auth::has_permission_to(const std::string& role_name,
                                   const eows::core::http_request& request,
                                   eows::core::http_response& response)
{
  std::vector<std::string> roles;
  roles.push_back(role_name);

  return has_permission_to(std::move(roles), request, response);
}

bool eows::auth::has_permission_to(const std::vector<std::string> roles,
                                   const eows::core::http_request& request,
                                   eows::core::http_response& response)
{
  std::string error_code;
  try
  {
    eows::core::authorization_t authorization = eows::core::authorization(request);

    if (authorization.value.empty())
      throw invalid_token_error("Unauthorized");

    token_t token_handler(authorization.value);

    if (authorization.type != eows::core::authorization_t::type_t::bearer)
      throw invalid_request_error("Unsupported authorization type. Expected 'Bearer'.");

    if (token_handler.expired())
      throw invalid_token_error("Token expired");

    const auto scope_str = token_handler.claim("scope");

    if (scope_str.empty())
      throw invalid_scope_error("No scope found");

    std::vector<std::string> scopes;
    eows::core::split(scope_str, ' ', scopes);

    for(const auto& role: roles)
    {
      auto found = std::find(scopes.begin(), scopes.end(), role);

      if (found == scopes.end())
        throw insufficient_scope_error("Insufficient scope. Requires \""+role+"\"");
    }

    response.set_status(eows::core::http_response::status_t::OK);
    return true;
  }
  catch(const invalid_request_error& e)
  {
    handle_oauth_error(e, response, eows::core::http_response::bad_request);
  }
  catch(const invalid_token_error& e)
  {
    handle_oauth_error(e, response, eows::core::http_response::unauthorized);
  }
  catch(const insufficient_scope_error& e)
  {
    handle_oauth_error(e, response, eows::core::http_response::forbidden);
  }

  const auto required_roles = eows::core::join(roles.begin(), roles.end(), std::string(" "));

  response.add_header(eows::core::http_response::WWW_AUTHENTICATE,
                      "Bearer,scope=\""+required_roles+"\",error="+error_code);
  return false;
}

void eows::auth::handle_oauth_error(const eows::auth::oauth2_error& e,
                                    eows::core::http_response& response,
                                    eows::core::http_response::status_t code)
{
  eows::auth::oauth_parameters params;
  params.error = e.error;
  params.error_description = e.error_description;

  const auto json = params.to_json();
  response.set_status(code);
  response.write(json.c_str(), json.size());

  response.add_header(eows::core::http_response::CONTENT_TYPE, "application/json");
}
