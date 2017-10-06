#include "authorization_code.hpp"
#include "generator.hpp"
#include "../data_types.hpp"
#include "exception.hpp"
#include "../manager.hpp"

#include "token.hpp"

// EOWS Core
#include "../../core/utils.hpp"

eows::auth::authorization_code::authorization_code(const oauth_parameters& p)
  : params_(p),
    generator_(new nonce_generator(64))
{
}

eows::auth::authorization_code::~authorization_code()
{
}

eows::auth::oauth_parameters eows::auth::authorization_code::grant(const eows::core::http_request& request,
                                                                   eows::core::http_response& response)
{
  if (params_.client_id.empty())
    throw unauthorized_error("The client identifier is empty.");

  oauth_parameters output;

  /*
   * Check Client ID, perform validations and then
   * fill output parameters
   */
  auto client = validate_client(params_.client_id, output);

  if (output.error.empty())
  {
    // Set state for CSRF attacks
    output.state = params_.state;

    // Set redirect URI
    output.redirect_uri = params_.redirect_uri;

    auto s = manager::instance().find_session(request, response);

    if (s->user.empty())
      throw unauthorized_error("User is not authenticated");

    // Validate credentials, code and authenticate user
    auto user = manager::instance().find_user(s->user);

    // Get Roles. TODO: Browser may encode space as "%20" or "+" for spaces. We should transform in single pattern
    std::vector<std::string> roles;
    if (params_.code.empty())
      eows::core::split(params_.scope, '+', roles);
    else
    {
      // Retrieve default roles of Generated Code
      auto code = manager::instance().find_code(params_.code);

      if (code == nullptr)
        throw access_denied_error("Invalid code provided");

      roles = code->roles;
    }

    if (!validate_roles(output, roles, *client))
      throw invalid_scope_error("The scope provided is invalid.");

    std::unique_ptr<oauth_code> code(new oauth_code);
    code->id = generator_->generate();
    code->expiration = eows::core::unix_timestamp() + manager::instance().settings().oauth2_code_expiration; // Last a //day// minute
    code->redirect_uri = output.redirect_uri;
    code->user_id = user->username;
    code->client_id = params_.client_id;
    code->roles = roles;

    output.code = code->id;
    params_.code = output.code;
    params_.username = user->username;

    manager::instance().create_code(std::move(code));

    s->user = user->username;
    s->roles.add("oauth2");
    s->roles.set("oauth2", "username", user->username);
    for(const auto& role: roles)
    {
      s->roles.add(role);
      s->roles.set(role, "username", user->username);
    }
  }

  return output;
}

void eows::auth::authorization_code::exchange(eows::auth::oauth_parameters& oresp,
                                              const eows::core::http_request& request,
                                              eows::core::http_response& response)
{
  // Current implementation only support Authorization code Grant Type
  if (params_.grant_type != "authorization_code")
    throw unsupported_response_type_error("The grant type provided is not supported");

  if (params_.client_id.empty())
    throw unauthorized_error("The client id was not provided");

  /* TODO: Validate Client. Remember that client may send only client secret or both (client id and secret).*/
  validate_client(params_.client_id, oresp);

  // Once grant_type is refresh_token, we should use grant type code as we'll use same resources
  bool refresh_token = false;

  if (params_.grant_type == "refresh_token")
  {
    params_.grant_type = "code";
    params_.code = params_.refresh_token;
    refresh_token = true;
  }

  auto code = manager::instance().find_code(params_.code);

  if (code == nullptr)
    throw unauthorized_error("The code provided is invalid");
  else
  {
    if (refresh_token)
    {
      // The token is still valid.. TODO: Should reply same token or generate a new one?
      create_access_token(oresp, code->user_id, code->roles);
    }
    else
    {
      if (code->expired())
        throw unauthorized_error("The code provided has expired");

      // Creating token
      create_access_token(oresp, code->user_id, code->roles);

      // Change code by refresh_token
      code->id = oresp.refresh_token;
    }
  }
}

eows::auth::oauth_client* eows::auth::authorization_code::validate_client(const std::string& client_id,
                                                                          eows::auth::oauth_parameters& oresp)
{
  // Find Client in cache/database | Validate
  auto client = manager::instance().find_client(client_id);

  if (client == nullptr)
    throw invalid_request_error("The client identifier provided is not valid");
  else
  {
    // If found, validate redirect URI
    if (!params_.redirect_uri.empty())
    {
      // Validate URI
      if (!client->has_redirect_uri(params_.redirect_uri))
        throw invalid_request_error("The redirect URI \"" + params_.redirect_uri + "\" is not in client app.");
      else
      {
        if (params_.response_type != "code" &&
            params_.grant_type != "authorization_code")
          throw unsupported_response_type_error("The response grant type not supported");
      }
    }
    else
    {
      // Retrieve first URI of client
      params_.redirect_uri = client->roles[0];
    }
  }

  return client;
}

bool eows::auth::authorization_code::validate_roles(eows::auth::oauth_parameters& oresp,
                                                    std::vector<std::string>& roles,
                                                    const oauth_client& client)
{
  bool has_role = true;
  for(const auto& role: roles)
    if (!client.has_role(role))
    {
      has_role = false;
      break; // TODO: throw exception
    }
  return has_role;
}

void eows::auth::authorization_code::create_access_token(eows::auth::oauth_parameters& oresp,
                                                         const std::string& user,
                                                         const std::vector<std::string>& roles)
{
  const auto now = std::time(nullptr);
  const auto token_expiration = now + manager::instance().settings().session_expiration;

  std::string scope = eows::core::join(roles.begin(), roles.end(), std::string(" "));

  token_t::metadata_t token_metadata;
  token_metadata.insert(std::make_pair("exp", std::to_string(token_expiration)));
  token_metadata.insert(std::make_pair("scope", scope));
  token_metadata.insert(std::make_pair("username", user));
  token_metadata.insert(std::make_pair("iat", std::to_string(now)));

  /*
   * A Refresh token never expires, but it need contains scope information
   * to re-generate with same contexts.
   * The OAuth 2 servers providers recommends that the new tokens should be issued
   * with a short expiration time so that applications is forced to continually refresh
   * them, giving the service a change to revoke application's access if needed.
   *
   * See more in https://www.oauth.com/oauth2-servers/access-tokens/access-token-lifetime/
   */
  token_t::metadata_t refresh_metadata;
  refresh_metadata.insert(std::make_pair("scope", scope));

  token_t handler(token_metadata);

  token_t refresh_token(refresh_metadata);

  oresp.access_token = handler.token();
  oresp.token_type = "Bearer";
  oresp.expires_in = std::to_string(token_expiration);
  oresp.state = params_.state;
  oresp.refresh_token = refresh_token.token();
}
