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

// EOWS Core
#include "../core/logger.hpp"
#include "../core/service_operations_manager.hpp"
#include "../core/utils.hpp"

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
        "/template",
        std::unique_ptr<eows::auth::dummy_login_handler>(new eows::auth::dummy_login_handler));

  eows::core::service_operations_manager::instance().insert(
        "/login",
        std::unique_ptr<eows::auth::oauth2_login_handler>(new eows::auth::oauth2_login_handler));
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
unsigned char *text_to_encrypt = new unsigned char[text_size]();
  memcpy(text_to_encrypt,text.c_str(),text.size()+1);

const int key_size = password.size() + 1;
unsigned char *key = new unsigned char[key_size]();

  memcpy(key,password.c_str(),password.size()+1);
unsigned char enc_out[256];
  AES_KEY enc_key;
  AES_set_encrypt_key(key, 256, &enc_key);
  AES_encrypt(text_to_encrypt, enc_out, &enc_key);

delete[] text_to_encrypt;
delete[] key;
  return std::string(reinterpret_cast<const char*>(enc_out),256);
//  password += "qwertyuiopasdfghjklzxcvbnmqwertyqwertyuiopasdfghjklzxcvbnmqwerty";

//  const int text_size = text.size() + 1;
//  std::unique_ptr<unsigned char> text_to_encrypt(new unsigned char[text_size]);
//  std::memcpy(text_to_encrypt.get(), text.c_str(), text.size() + 1);

//  const int key_size = password.size() + 1;
//  std::unique_ptr<unsigned char> key(new unsigned char[key_size]);

//  std::memcpy(key.get(), password.c_str(), password.size() + 1);
//  unsigned char enc_out[256];
//  AES_KEY enc_key;
//  AES_set_encrypt_key(key.get(), 256, &enc_key);
//  AES_encrypt(text_to_encrypt.get(), enc_out, &enc_key);

//  return std::string(reinterpret_cast<const char*>(enc_out),256);
}

std::string eows::auth::decrypt(const std::string& text, std::string password)
{
  password += "qwertyuiopasdfghjklzxcvbnmqwertyqwertyuiopasdfghjklzxcvbnmqwerty";

const int key_size = password.size() + 1;
unsigned char *key = new unsigned char[key_size]();
  memcpy(key,password.c_str(),password.size()+1);

  unsigned char dec_out[256];
  AES_KEY dec_key;
  AES_set_decrypt_key(key,256,&dec_key);

  unsigned char crypted_text[256];
  memcpy(crypted_text,text.c_str(),256);
  AES_decrypt(crypted_text, dec_out, &dec_key);

delete[] key;

  return std::string(reinterpret_cast<const char*>(dec_out));
//  password += "qwertyuiopasdfghjklzxcvbnmqwertyqwertyuiopasdfghjklzxcvbnmqwerty";

//  const int key_size = password.size() + 1;
//  std::unique_ptr<unsigned char> key(new unsigned char[key_size]);
//  std::memcpy(key.get(), password.c_str(), password.size()+1);

//  unsigned char dec_out[256];
//  AES_KEY dec_key;
//  AES_set_decrypt_key(key.get(), 256, &dec_key);

//  unsigned char crypted_text[256];
//  memcpy(crypted_text, text.c_str(), 256);
//  AES_decrypt(crypted_text, dec_out, &dec_key);

//  return std::string(reinterpret_cast<const char*>(dec_out));
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

//std::string eows::auth::generate(const int& length)
//{
//  unsigned char nonce[length];
//  int rc = RAND_bytes(nonce, sizeof(nonce));
////  unsigned long err = ERR_get_error();

//  if (rc != 1) {
//    // THROW error

////    unsigned long err = ERR_get_error();
//  }
//  return to_utf8string(std::string(reinterpret_cast<const char*>(nonce)));
//}
