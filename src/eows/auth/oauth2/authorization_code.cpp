#include "authorization_code.hpp"
#include "../exception.hpp"
#include "../manager.hpp"

eows::auth::authorization_code::authorization_code(const oauth_parameters& p)
  : params_(p)
{
}

eows::auth::oauth_parameters eows::auth::authorization_code::grant(const eows::core::http_request& request, eows::core::http_response& response)
{
  if (params_.client_id.empty())
    throw eows_error("No client id given");

  oauth_parameters output;

  // Once grant_type is refresh_token, we should use grant type code as we'll use same resources
  if (params_.grant_type == "refresh_token")
  {
    params_.grant_type = "code";
    params_.code = params_.refresh_token;
  }

  /*
   * Check Client ID, perform validations and then
   * fill output parameters
   */
  validate_client(params_.client_id, output);

  if (params_.response_type.empty())
    throw eows_error("Missing response_type parameter");
  /*Value MUST be set to "code" or one of the OpenID authorization code including
    response_types "code token", "code id_token", "code token id_token"*/
  else if (params_.response_type != "none" /*&& ! "code" in response_type !*/)
    throw eows_error("Unsupported ");

  // Validate OAuth Scopes
  validate_scope(output);

  return output;
}

const std::string eows::auth::authorization_code::information()
{
  return std::string();
}

void eows::auth::authorization_code::validate_client(const std::string& client_id, eows::auth::oauth_parameters& oresp)
{
  // Find Client in cache/database | Validate

  // If found, validate redirect URI
  if (!params_.redirect_uri.empty())
  {
    // Validate URI
  }
  else
  {
    oresp.redirect_uri = ""; // Get default redirect URI. If empty, throw error
  }
}

void eows::auth::authorization_code::validate_scope(eows::auth::oauth_parameters& oresp)
{

}
