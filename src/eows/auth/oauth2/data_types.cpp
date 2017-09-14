#include "data_types.hpp"

bool has(const std::vector<std::string> arr, const std::string& str)
{
  for(const auto& r: arr)
    if (r == str)
      return true;
  return false;
}

eows::auth::oauth_parameters::oauth_parameters(const eows::core::query_string_t& query_string)
{
  configure(query_string);
}

void eows::auth::oauth_parameters::configure(const eows::core::query_string_t& query_string)
{
  set_property(query_string, "authorize", authorize);
  set_property(query_string, "code", code);
  set_property(query_string, "username", username);
  set_property(query_string, "password", password);
  set_property(query_string, "access_token", access_token);
  set_property(query_string, "expires_in", expires_in);
  set_property(query_string, "refresh_token", refresh_token);
  set_property(query_string, "token_type", token_type);
  set_property(query_string, "grant_type", grant_type);
  set_property(query_string, "response_type", response_type);
  set_property(query_string, "client_id", client_id);
  set_property(query_string, "client_secret", client_secret);
  set_property(query_string, "redirect_uri", redirect_uri);
  set_property(query_string, "scope", scope);
  set_property(query_string, "state", state);
  set_property(query_string, "error", error);
  set_property(query_string, "error_description", error_description);
}

void eows::auth::oauth_parameters::clear()
{
  authorize.clear();
  code.clear();
  username.clear();
  password.clear();
  access_token.clear();
  expires_in.clear();
  refresh_token.clear();
  token_type.clear();
  grant_type.clear();
  response_type.clear();
  client_id.clear();
  client_secret.clear();
  redirect_uri.clear();
  scope.clear();
  state.clear();
  error.clear();
  error_description.clear();
}

const std::string eows::auth::oauth_parameters::to_json() const
{
  std::string json;

  if (!code.empty())
    json += ",\"code\":\"" + code + "\"";

  if (!access_token.empty())
    json += ",\"access_token\":\"" + access_token + "\"";

  if (!expires_in.empty())
    json += ",\"expires_in\": " + expires_in;

  if (!refresh_token.empty())
    json += ",\"refresh_token\":\"" + refresh_token + "\"";

  if (!token_type.empty())
    json += ",\"token_type\":\"" + token_type + "\"";

  if (!grant_type.empty())
    json += ",\"grant_type\":\"" + grant_type + "\"";

  if (!response_type.empty())
    json += ",\"response_type\":\"" + response_type + "\"";

  if (!client_id.empty())
    json += ",\"client_id\":\"" + client_id + "\"";

  if (!redirect_uri.empty())
    json += ",\"redirect_uri\":\"" + redirect_uri + "\"";

  if (!scope.empty())
    json += ",\"scope\":\"" + scope + "\"";

  if (!state.empty())
    json += ",\"state\":\"" + state + "\"";

  if (!error.empty())
    json +=",\"error\":\""+error+"\"";

  if (!error_description.empty())
    json +=",\"error_description\":\""+error_description+"\"";

  if (json.empty())
    json = "{}";
  else
  {
    json[0] = '{';
    json += "}";
  }

  return json;
}

const eows::core::query_string_t eows::auth::oauth_parameters::to_query_string() const
{
  eows::core::query_string_t output;
  if (!code.empty())
    output.insert(eows::core::query_string_t::value_type("code", code));
  if (!access_token.empty())
    output.insert(eows::core::query_string_t::value_type("access_token", access_token));
  if (!expires_in.empty())
    output.insert(eows::core::query_string_t::value_type("expires_in", expires_in));
  if (!refresh_token.empty())
    output.insert(eows::core::query_string_t::value_type("refresh_token", refresh_token));
  if (!token_type.empty())
    output.insert(eows::core::query_string_t::value_type("token_type", token_type));
  if (!grant_type.empty())
    output.insert(eows::core::query_string_t::value_type("grant_type", grant_type));
  if (!response_type.empty())
    output.insert(eows::core::query_string_t::value_type("response_type", response_type));
  if (!client_id.empty())
    output.insert(eows::core::query_string_t::value_type("client_id", client_id));
  if (!redirect_uri.empty())
    output.insert(eows::core::query_string_t::value_type("redirect_uri", redirect_uri));
  if (!scope.empty())
    output.insert(eows::core::query_string_t::value_type("scope", scope));
  if (!state.empty())
    output.insert(eows::core::query_string_t::value_type("state", state));
  if (!error.empty())
    output.insert(eows::core::query_string_t::value_type("error", error));
  if (!error_description.empty())
    output.insert(eows::core::query_string_t::value_type("error_description", error_description));
  return output;
}

void eows::auth::oauth_parameters::set_property(const eows::core::query_string_t& query_string, const std::string& key, std::string& target)
{
  auto it = query_string.find(key);

  if (it != query_string.end())
    target.assign(it->second);
}

bool eows::auth::oauth_client::has_redirect_uri(const std::string& uri) const
{
  return has(redirect_uris, uri);
}

bool eows::auth::oauth_client::has_role(const std::string& role) const
{
  return has(roles, role);
}

bool eows::auth::role_map::has_role(const std::string& role_name)
{
  return roles.find(role_name) != roles.end();
}

bool eows::auth::role_map::has_role(const std::string& role_name, const std::string& key)
{
  auto it = roles.find(role_name);
  if (it != roles.end()){
    const std::map<std::string,std::string>& properties = it->second;
    auto it2 = properties.find(key);
    if(it2 != properties.end()){
      return true;
    }
  }
  return false;

}

bool eows::auth::role_map::add(const std::string& role)
{
  if (!has_role(role))
  {
    set(role, "0", "0");
    return true;
  }
  return false;
}

void eows::auth::role_map::set(const std::string& role, const std::string& key, const std::string& value)
{
  auto it = roles.find(role);
  if (it == roles.end())
  {
    std::map<std::string,std::string> properties;
    properties[key] = value;
    roles[role] = properties;
  }
  else
  {
    std::map<std::string,std::string> properties = it->second;
    properties[key] = value;
    roles[role] = properties;
  }
}

void eows::auth::role_map::remove(const std::string& role)
{
  roles.erase(role);
}

void eows::auth::role_map::remove(const std::string& role, const std::string& key)
{
  auto it = roles.find(role);
  if (it != roles.end()){
    std::map<std::string,std::string> properties = it->second;
    properties.erase(key);
    roles[role] = properties;
  }
}

bool is_expired_from_now(std::time_t tm)
{
  std::time_t now = std::time(nullptr);
  long int seconds = (long int)std::difftime(now, tm);

  return seconds > 0;
}

//bool eows::auth::session::has_role(const std::string& role_name)
//{
//  return has(roles, role_name);
//}

bool eows::auth::session::expired() const
{
  return is_expired_from_now(update_time);
}

bool eows::auth::oauth_code::expired() const
{
  return is_expired_from_now(expiration);
}
