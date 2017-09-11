#include "authorization_code.hpp"
#include "utils.hpp"
#include "../data_types.hpp"
#include "../exception.hpp"
#include "../manager.hpp"

// EOWS Core
#include "../../core/utils.hpp"

eows::auth::authorization_code::authorization_code(const oauth_parameters& p)
  : params_(p)
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
      }

      if (validate_roles(output, roles, *client))
      {
        if (params_.response_type == "code")
        {
          output.code = "somecode";
          output.code = user->username;
          output.client_id = params_.client_id;
          output.response_type = params_.response_type;
          output.scope = params_.scope;

          session* s = manager::instance().find_session(request, response);
          s->user = user->username;
          s->roles = roles;
          // Create Code
//          create_code(output, request, response);
//          params_.code = client_code.code;
//          params_.username = client_code.username;
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

eows::auth::oauth_parameters eows::auth::authorization_code::exchange(eows::auth::oauth_parameters& oresp, const eows::core::http_request& request, eows::core::http_response& response)
{
  // Once grant_type is refresh_token, we should use grant type code as we'll use same resources
  if (params_.grant_type == "refresh_token")
  {
    params_.grant_type = "code";
    params_.code = params_.refresh_token;
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
        oresp.redirect_uri = params_.redirect_uri;

        if (params_.response_type != "code" &&
            params_.grant_type != "authorization_code")
          unsupported(oresp);
      }
    }
    else
      oresp.redirect_uri = ""; // Get default redirect URI. If empty, throw error
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
