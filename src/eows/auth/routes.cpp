#include "routes.hpp"
#include "manager.hpp"
#include "data_types.hpp"
#include "utils.hpp"

// EOWS OAuth
#include "oauth2/data_types.hpp"
#include "oauth2/authorization_code.hpp"

// EOWS Core
#include "../core/utils.hpp"
#include "../core/http_request.hpp"
#include "../core/http_response.hpp"

void reply(eows::core::http_response& res, const eows::auth::oauth_parameters& parameters, std::string tpl, eows::core::http_response::status_t status_code)
{
  res.set_status(status_code);
  res.add_header(eows::core::http_response::CONTENT_TYPE, "text/html; charset=utf-8");

  eows::auth::replace(tpl, parameters.to_query_string());

  res.write(tpl.c_str(), tpl.size());
}

void forbidden(eows::core::http_response& res, eows::auth::oauth_parameters& parameters)
{
  parameters.clear();
  parameters.error = "access_forbidden";
  parameters.error_description = "Access restrict";

  reply(res, parameters,
        eows::auth::manager::instance().error_template(),
        eows::core::http_response::forbidden);
}

//void move(eows::core::http_response& res, const std::string& location, eows::auth::oauth_parameters& parameters)
//{
//  const auto query_string = eows::core::to_str(parameters.to_query_string());

//  res.add_header(eows::core::http_response::LOCATION, location+query_string);
//}

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

  // Check required OAuth2 parameters
  if (input_params.error.empty() &&  (input_params.response_type.empty() || input_params.client_id.empty()))
    return forbidden(res, input_params);

  // Check for Authorization roles in session/DB
  auto s = manager::instance().find_session(req, res);

  // Make sure session found or parameters valid. Otherwise, reply login page
  if (s == nullptr || s->user == "" || input_params.scope.empty())
  // TODO Validate all parameters for login
    return reply(res, input_params,
                 manager::instance().login_template(),
                 eows::core::http_response::OK);

  // Make sure the specified role is in app
  if (!s->has_role(input_params.scope))
    return forbidden(res, input_params);

  // OK, everything is fine
  return reply(res, input_params,
               manager::instance().authorize_template(),
               eows::core::http_response::OK);
}

void eows::auth::oauth2_authorize::do_post(const eows::core::http_request& req, eows::core::http_response& res)
{
  oauth_parameters input_params(req.data());
  authorization_code authorization(input_params);

  auto output_params = authorization.grant(req, res);

  if (output_params.redirect_uri.empty())
  {
    // Retrieve from HTTP Referer
    output_params.redirect_uri = referer(req);
  }

  res.set_status(eows::core::http_response::moved_permanently);
  const std::string redirect_uri = output_params.redirect_uri;
  output_params.redirect_uri.clear();
  res.add_header(eows::core::http_response::LOCATION, redirect_uri + eows::core::to_str(output_params.to_query_string()));
}

void eows::auth::oauth2_info::do_get(const eows::core::http_request& req, eows::core::http_response& res)
{

}

void eows::auth::oauth2_logout::do_get(const eows::core::http_request& req, eows::core::http_response& res)
{

}

void eows::auth::dummy::do_get(const eows::core::http_request& req, eows::core::http_response& res)
{
  const std::string url = "/oauth2/authorize?response_type=code&client_id=some_id&scope=user.email&redirect_uri=http://localhost:7654/echo";
  const std::string html = "<a target=\"_blank\""
                           "href=\""+ url +"\">Log in with E-Sensing EOWS</a>";
  res.set_status(eows::core::http_response::OK);
  res.add_header(eows::core::http_response::CONTENT_TYPE, "text/html; charset=utf-8");
  res.write(html.c_str(), html.size());
}

void eows::auth::oauth2_token::do_post(const eows::core::http_request& req, eows::core::http_response& res)
{
//  TODO
  oauth_parameters input_params(req.data());
  authorization_code authorization(input_params);

  // For exchanging code to an access_token
  if (input_params.grant_type == "authorization_code")
  {
    oauth_parameters oresp;
    authorization.exchange(oresp, req, res);
    /*
     *
     * TODO: Make an request to exchange code for access_token
     * just for testing purpose, since the it is user's requirement
     *
     */

    res.set_status(eows::core::http_response::OK);
    const auto json = oresp.to_json();
    return res.write(json.c_str(), json.size());
  }
  // 403
  return forbidden(res, input_params);
}
