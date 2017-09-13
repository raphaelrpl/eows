#include "authorization_code.hpp"
#include "utils.hpp"
#include "generator.hpp"
#include "../data_types.hpp"
#include "exception.hpp"
#include "../manager.hpp"

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

eows::auth::oauth_parameters eows::auth::authorization_code::grant(const eows::core::http_request& request, eows::core::http_response& response)
{
  if (params_.client_id.empty())
    throw eows_error("No client id given");

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

    // Validate credentials, code and authenticate user
    user_t* user = validate_credentials(output, request, response);

    // On no error,
    if (output.error.empty())
    {
      // Get Roles. TODO: Browser may encode space as "%20" or "+" for spaces. We should transform in single pattern
      std::vector<std::string> roles;
      if (params_.code.empty())
        eows::core::split(params_.scope, '+', roles);
      else
      {
        // Retrieve default roles of Generated Code
        // auto code = manager::instance().find_code(params_.code);
        // roles = code->roles();
      }

      if (validate_roles(output, roles, *client))
      {
        if (params_.response_type == "code")
        {
          std::unique_ptr<oauth_code> code(new oauth_code);
          code->id = generator_->generate();
          code->expiration = eows::core::unix_timestamp() + 60; // Last a minute
          code->redirect_uri = output.redirect_uri;
          code->user_id = user->username;
          code->client_id = params_.client_id;

          output.code = code->id;
          params_.code = output.code;
          params_.username = user->username;

          manager::instance().create_code(std::move(code));
//          output.client_id = params_.client_id;
//          output.response_type = params_.response_type;
//          output.scope = params_.scope;

          session* s = manager::instance().find_session(request, response);
          s->user = user->username;
//          s->roles = roles;
//          for(const auto& role: roles)
//          {
//            s->roles.set(role, "username", user->username);
//            s->roles.set(role, "");
//          }
        }
        else
        {
          // Create Access Token
        }
      }
    }
  }

  return output;
}

void eows::auth::authorization_code::exchange(eows::auth::oauth_parameters& oresp, const eows::core::http_request& request, eows::core::http_response& response)
{
  // Once grant_type is refresh_token, we should use grant type code as we'll use same resources

  bool refresh_token = false;

  oauth_code* code = nullptr;

  if (params_.grant_type == "refresh_token")
  {
    params_.grant_type = "code";
    params_.code = params_.refresh_token;
    refresh_token = true;
  }

  code = manager::instance().find_code(params_.code);

  if (code == nullptr)
    throw unauthorized_error("The code provided is invalid");
  else
  {
    if (code->expired())
      throw unauthorized_error("The code provided has expired");

    if (refresh_token)
    {
      // The token is still valid.. TODO: Should reply same token or generate a new one?
    }
    else
    {
      // Creating token
      nonce_generator g(86);
      oresp.access_token = g.generate();
      oresp.token_type = "Bearer";
      oresp.expires_in = "86400"; /* day */
      oresp.state = params_.state;
      oresp.refresh_token = generator_->generate();

      auto s = manager::instance().find_session(request, response);
//      s->roles.
    }
  }
}

const std::string eows::auth::authorization_code::information()
{
  return std::string();
}

eows::auth::oauth_client* eows::auth::authorization_code::validate_client(const std::string& client_id, eows::auth::oauth_parameters& oresp)
{
  // Find Client in cache/database | Validate
  auto client = manager::instance().find_client(client_id);

  if (client == nullptr)
    invalid_request(oresp, "No client found");
  else
  {
    // If found, validate redirect URI
    if (!params_.redirect_uri.empty())
    {
      // Validate URI
      if (!client->has_redirect_uri(params_.redirect_uri))
        invalid_request(oresp, "No redirect uri found in client");
      else
      {
//        oresp.redirect_uri = params_.redirect_uri;

        if (params_.response_type != "code" &&
            params_.grant_type != "authorization_code")
          unsupported(oresp);
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

eows::auth::user_t* eows::auth::authorization_code::validate_credentials(eows::auth::oauth_parameters& oresp,
                                                                         const eows::core::http_request& request,
                                                                         eows::core::http_response& response)
{
//  /*
//   * If GrantType is AuthorizationCode, then the user is exchanging a code for access_token
//   */
//  if (params_.grant_type == "authorization_code")
//  {
//    if (params_.code.empty())
//      return access_denied(oresp);
//    // Retrieve Access Token from Code

//    /*
//     * Find for user access token using generated code before
//     *
//     * {
//     *   "code": "someHashUsedToRetrieveAccessToken",
//     *   "state": "someHashCRSFToken"
//     * }
//     */
//  }
//  else
//  {
  if (!params_.client_secret.empty()) // && is_valid_secret(params_.client_secret))
  {
    unauthorized(oresp);
    return nullptr;
  }

  if (!params_.authorize.empty() && params_.authorize == "authorize")
  {
    // Retrieve Session from request/response
    user_t* user = manager::instance().find_user(params_.username);
//    session* s = manager::instance().find_session(request, response);

    if (user == nullptr || user->password != params_.password)
    {
      access_denied(oresp);
      return nullptr;
    }

    return user;

//    if (s == nullptr)
//    {
////      return access_denied(oresp);

//      if (params_.username.empty() || params_.password.empty())
//        return access_denied(oresp);

//      user_t* user = manager::instance().find_user(params_.username);

//      // Create session
//      manager::instance().create_session(*user);
//      return;
//    }

//    user_t* u = manager::instance().find_user(s->user);

//    if (u == nullptr)
//      return access_denied(oresp);
  }
  else
  {
    if (params_.username.empty() || params_.password.empty())
    {
      access_denied(oresp);
      return nullptr;
    }

    // Validate User Credentials
    //
    user_t* user = manager::instance().find_user(params_.username);
    //
    if (user == nullptr || user->password != params_.password) // We should compare password using password hash
    {
      access_denied(oresp);
      return nullptr;
    }

    return user;

    // Create session
//    manager::instance().create_session(*user);
  }
//  }
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
