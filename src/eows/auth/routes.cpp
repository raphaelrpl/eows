#include "routes.hpp"
#include "manager.hpp"
#include "data_types.hpp"
#include "utils.hpp"

// EOWS OAuth
#include "oauth2/data_types.hpp"
#include "oauth2/authorization_code.hpp"
#include "oauth2/exception.hpp"

// EOWS Core
#include "../core/utils.hpp"
#include "../core/http_request.hpp"
#include "../core/http_response.hpp"

void reply(eows::core::http_response& res,
           const eows::core::query_string_t& parameters,
           std::string tpl,
           eows::core::http_response::status_t status_code)
{
  res.set_status(status_code);
  res.add_header(eows::core::http_response::CONTENT_TYPE, "text/html; charset=utf-8");

  eows::auth::replace(tpl, parameters);

  res.write(tpl.c_str(), tpl.size());
}

//! Helper to reply a HTTP response for OAuth2 model.
void reply(eows::core::http_response& res,
           const eows::auth::oauth_parameters& parameters,
           std::string tpl,
           eows::core::http_response::status_t status_code)
{
  reply(res, parameters.to_query_string(), tpl, status_code);
}

//! Helper to reply forbidden page on HTTP OAuth2 Requests
void forbidden(eows::core::http_response& res, eows::auth::oauth_parameters& parameters)
{
  parameters.clear();
  parameters.error = "access_forbidden";
  parameters.error_description = "Access restrict";

  reply(res, parameters,
        eows::auth::manager::instance().error_template(),
        eows::core::http_response::forbidden);
}

void handle_oauth_error(eows::core::http_response& res,
                        eows::core::http_response::status_t status_code,
                        eows::auth::oauth_parameters& params,
                        const eows::auth::oauth2_error& e)
{
  params.clear();
  params.error = e.error;
  params.error_description = e.error_description;
  res.set_status(status_code);
}

void eows::auth::oauth2_authorize::do_get(const eows::core::http_request& req, eows::core::http_response& res)
{
  oauth_parameters input_params(req.query_string());

  res.add_header(eows::core::http_response::ACCESS_CONTROL_ALLOW_CREDENTIALS, "true");

  // Check required OAuth2 parameters
  if (input_params.error.empty() &&  (input_params.response_type.empty() || input_params.client_id.empty()))
    return forbidden(res, input_params);

  if (!input_params.error.empty())
    return forbidden(res, input_params);

  // Check for Authorization roles in session/DB
  auto s = manager::instance().find_session(req, res);

  // Make sure session found or parameters valid. Otherwise, reply login page
  if (s == nullptr || s->user == "" || input_params.scope.empty())
  {
    std::string redirect = input_params.redirect_uri;
    redirect += eows::core::to_str(input_params.to_query_string());

    input_params.redirect_uri = redirect;
    // TODO Validate all parameters for login
    return reply(res, input_params,
                 manager::instance().login_template(),
                 eows::core::http_response::OK);
  }

  // Make sure the specified role is in app
  if (!s->roles.has_role("oauth2"))
    return forbidden(res, input_params);

  std::vector<std::string> roles;
  eows::core::split(input_params.scope, '+', roles);

  bool has_all_roles = true;
  for(const auto& role: roles)
  {
    if (!s->roles.has_role(role))
    {
      has_all_roles = false;
      break;
    }
  }

  if (has_all_roles)
  {
    const auto redirect_uri = input_params.redirect_uri;
    input_params.redirect_uri = "";
    // redirect URI
    res.add_header(eows::core::http_response::ACCESS_CONTROL_ALLOW_CREDENTIALS, "true");
    return res.redirect_to(redirect_uri + eows::core::to_str(input_params.to_query_string()));
  }

  eows::core::query_string_t parameters = input_params.to_query_string();
  std::string html_tpl("<ul class=\"list-group\">");

  for(const auto role: roles)
  {
    html_tpl.append("<li class=\"list-group-item\">");
    html_tpl.append("<span class=\"glyphicon glyphicon-ok\"></span> ");
    html_tpl.append(role);
  }

  html_tpl.append("</ul>");

  parameters.insert(std::make_pair("content", html_tpl));

  auto client = manager::instance().find_client(input_params.client_id);
  parameters.insert(std::make_pair("app_name", client->application_name));

  // OK, everything is fine
  return reply(res, parameters,
               manager::instance().authorize_template(),
               eows::core::http_response::OK);
}

void eows::auth::oauth2_authorize::do_post(const eows::core::http_request& req, eows::core::http_response& res)
{
  oauth_parameters input_params(req.data());

  // Current implementation only support Authorization code Grant Type
  if (input_params.response_type != "code")
    throw unsupported_response_type_error("The response type provided is not supported");

  authorization_code authorization(input_params);
  oauth_parameters output_params;

  try
  {
    output_params = authorization.grant(req, res);

    if (output_params.redirect_uri.empty())
    {
      // Retrieve from HTTP Referer
      output_params.redirect_uri = eows::core::referer(req);
    }

    const std::string redirect_uri = output_params.redirect_uri;
    output_params.redirect_uri.clear();
    return res.redirect_to(redirect_uri + eows::core::to_str(output_params.to_query_string()));
  }
  catch(const eows::auth::unauthorized_error& e)
  {
    handle_oauth_error(res, eows::core::http_response::unauthorized, output_params, e);
  }
  catch(const eows::auth::invalid_request_error& e)
  {
    handle_oauth_error(res, eows::core::http_response::bad_request, output_params, e);
  }
  catch(const eows::auth::access_denied_error& e)
  {
    handle_oauth_error(res, eows::core::http_response::forbidden, output_params, e);
  }
  catch(const eows::auth::temporarily_unavailable_error& e)
  {
    handle_oauth_error(res, eows::core::http_response::service_unavailable, output_params, e);
  }
  // Handle both any exception or server_error, catch as internal server error
  catch(const eows::auth::oauth2_error& e)
  {
    handle_oauth_error(res, eows::core::http_response::internal_server_error, output_params, e);
  }

  const auto json_err = output_params.to_json();
  res.write(json_err.c_str(), json_err.size());
  res.add_header(eows::core::http_response::CONTENT_TYPE, "application/json;charset=utf-8");
}

void eows::auth::oauth2_info::do_get(const eows::core::http_request& req, eows::core::http_response& res)
{
  auto headers = req.headers();
  auto it = headers.find("Authorization");

  if (it == headers.end())
  {
    oauth_parameters error_params;
    handle_oauth_error(res, eows::core::http_response::unauthorized, error_params, eows::auth::unauthorized_error("No authorization provided"));
  }

  eows::core::authorization_t auth_header(it->second);

  const std::string token = auth_header.value;

  /*
    Find Session With OAuth2 Token

    Find User associated with Token

    Serialize User as JSON
  */
  const std::string response("{}");

  res.write(response.c_str(), response.size());
  res.add_header(eows::core::http_response::CONTENT_TYPE, "application/json;charset=utf-8");
}

void eows::auth::oauth2_logout::do_get(const eows::core::http_request& req, eows::core::http_response& res)
{
  auto s = manager::instance().find_session(req, res);
  manager::instance().remove_session(s);

  std::string msg("You have been disconnected");

  // Setting cookie with session_id expired
  res.add_header(eows::core::http_response::SET_COOKIE, "session_id=deleted,expires=Thu, 01 Jan 1970 00:00:00 GMT");
  res.add_header(eows::core::http_response::CONTENT_TYPE, "text/html; charset=utf-8");
  res.write(msg.c_str(), msg.size());
}

void eows::auth::oauth2_token_handler::do_post(const eows::core::http_request& req, eows::core::http_response& res)
{
//  TODO
  oauth_parameters input_params(req.data());
  authorization_code authorization(input_params);
  oauth_parameters oresp;

  try
  {
    // Exchange Code or Refresh token for An Access Token and write in oresp object
    authorization.exchange(oresp, req, res);
    res.set_status(eows::core::http_response::OK);
  }
  catch(const eows::auth::unauthorized_error& e)
  {
    handle_oauth_error(res, eows::core::http_response::unauthorized, oresp, e);
  }
  catch(const eows::auth::invalid_request_error& e)
  {
    handle_oauth_error(res, eows::core::http_response::bad_request, oresp, e);
  }
  catch(const eows::auth::access_denied_error& e)
  {
    handle_oauth_error(res, eows::core::http_response::forbidden, oresp, e);
  }
  catch(const eows::auth::temporarily_unavailable_error& e)
  {
    handle_oauth_error(res, eows::core::http_response::service_unavailable, oresp, e);
  }
  // Handle both any exception or server_error, catch as internal server error
  catch(const eows::auth::oauth2_error& e)
  {
    handle_oauth_error(res, eows::core::http_response::internal_server_error, oresp, e);
  }

  // Replying
  const auto json = oresp.to_json();
  res.add_header(eows::core::http_response::CONTENT_TYPE, "application/json; charset=utf-8");
  return res.write(json.c_str(), json.size());
}

void eows::auth::oauth2_login_handler::do_get(const eows::core::http_request& req, eows::core::http_response& res)
{
  auto s = manager::instance().find_session(req, res);

  if (s->expired() || !s->roles.has_role("oauth2"))
  {
    const auto query_string = req.query_string();

    auto it = query_string.find("redirectTo");
    oauth_parameters params;

    // Use OAuth 2.0 Template. Otherwise, render EOWS Login page
    if (it != query_string.end())
    {
      params.configure(eows::core::expand(it->second));

      // validate

      return reply(res, params,
                   manager::instance().login_template(),
                   eows::core::http_response::OK);
    }
    else
    {
      const std::string eows_login_template = "<h1>Login page</h1>";
      res.write(eows_login_template.c_str(), eows_login_template.size());
    }
  }
  else
    return res.redirect_to(eows::core::referer(req));
}

void eows::auth::oauth2_login_handler::do_post(const eows::core::http_request& req, eows::core::http_response& res)
{
  oauth_parameters params(req.query_string());

  const auto body = req.data();

  auto it = body.find("username");

  std::string username = it->second;

  it = body.find("password");

  std::string password = it->second;

  // Retrieve Session from request/response
  user_t* user = manager::instance().find_user(username);

  if (user == nullptr || user->password != password)
  {
    // Redirect to forbidden or even login again with errors
  }
  else
  {
    session* s = manager::instance().find_session(req, res);
    s->user = user->username;
    s->roles.add("oauth2");
    s->roles.set("oauth2", "username", user->username);
    params.client_id = body.find("client_id")->second;

    res.add_header(eows::core::http_response::ACCESS_CONTROL_ALLOW_CREDENTIALS, "true");
    return res.redirect_to("/oauth2/authorize" + eows::core::to_str(params.to_query_string()));
  }
}
