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

//! Helper to reply a HTTP response for OAuth2 model.
void reply(eows::core::http_response& res, const eows::auth::oauth_parameters& parameters, std::string tpl, eows::core::http_response::status_t status_code)
{
  res.set_status(status_code);
  res.add_header(eows::core::http_response::CONTENT_TYPE, "text/html; charset=utf-8");

  eows::auth::replace(tpl, parameters.to_query_string());

  res.write(tpl.c_str(), tpl.size());
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

/*!
 * \brief Retrieves a HTTP Referer from request. Used to redirect next uri
 *
 * The Referer request header contains the URI of the previous web page, allowing servers to
 * identify where people are visiting them FROM.
 *
 * See more in https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Referer
 *
 * \todo It must be moved to eows::core since it belongs to HTTP Request and there is no special behavior for retrieve URI handling.
 *
 * \param request
 * \return
 */
const std::string referer(const eows::core::http_request& request)
{
  std::string ref;
  const auto headers = request.headers();

  auto it = headers.find("Referer");
  if (it != headers.end())
    ref.assign(it->second);

  return ref;
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
//    input_params.redirect_uri = "";
    redirect += eows::core::to_str(input_params.to_query_string());

    input_params.redirect_uri = redirect;
    // TODO Validate all parameters for login
    return reply(res, input_params,
                 manager::instance().login_template(),
                 eows::core::http_response::OK);
  }

  // Make sure the specified role is in app
  if (!s->roles.has_role("oauth2"))
  {
    return forbidden(res, input_params);
  }

  // OK, everything is fine
  return reply(res, input_params,
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
      output_params.redirect_uri = referer(req);
    }

    res.set_status(eows::core::http_response::moved_permanently);
    const std::string redirect_uri = output_params.redirect_uri;
    output_params.redirect_uri.clear();
    return res.add_header(eows::core::http_response::LOCATION, redirect_uri + eows::core::to_str(output_params.to_query_string()));
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

void eows::auth::dummy_login_handler::do_get(const eows::core::http_request& req, eows::core::http_response& res)
{
  const std::string url = "/oauth2/authorize?response_type=code&client_id=some_id&scope=user.email&redirect_uri=http://127.0.0.1:7654/echo";
  const std::string html = "<a target=\"_blank\""
                           "href=\""+ url +"\">Log in with E-Sensing EOWS</a>";
  res.set_status(eows::core::http_response::OK);
  res.add_header(eows::core::http_response::CONTENT_TYPE, "text/html; charset=utf-8");
  res.write(html.c_str(), html.size());
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

void eows::auth::dummy_route::do_get(const eows::core::http_request& req, eows::core::http_response& res)
{
  auto s = manager::instance().find_session(req, res);

  std::string code, state;

  auto query_string = req.query_string();

  auto it = query_string.find("code");

  if (it != query_string.end())
    code.append(it->second);

  it = query_string.find("state");
  if (it != query_string.end())
    state.append(it->second);

  s->roles.add("oauth2");
  s->roles.set("oauth2", "state", state);

  // Request an access token
  // It should be called with HTTP Requester moduler. But in this example
  // We'gonna use direct call
}

void eows::auth::oauth2_login_handler::do_get(const eows::core::http_request& req, eows::core::http_response& res)
{
  auto s = manager::instance().find_session(req, res);

  if (s->expired() || !s->roles.has_role("oauth2"))
  {
    const auto query_string = req.query_string();

    auto it = query_string.find("redirectTo");
    oauth_parameters params;

    if (it != query_string.end())
    {
      params.configure(eows::core::expand(it->second));

      // validate

      return reply(res, params,
                   manager::instance().login_template(),
                   eows::core::http_response::OK);
    }
  }
  else
  {
    res.add_header(eows::core::http_response::LOCATION, referer(req));
  }
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
    res.add_header(eows::core::http_response::LOCATION, "/oauth2/authorize" + eows::core::to_str(params.to_query_string()));
    res.set_status(eows::core::http_response::moved_permanently);
  }
}
