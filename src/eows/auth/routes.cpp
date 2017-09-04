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
  const auto json = parameters.to_json();
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

void eows::auth::oauth2_authorize::do_get(const eows::core::http_request& req, eows::core::http_response& res)
{
  oauth_parameters input_params(req.query_string());
  oauth_parameters oresp;

  // Check required OAuth2 parameters
  if (input_params.error.empty() &&  (input_params.response_type.empty() || input_params.client_id.empty()))
    return forbidden(res, oresp);

  // Check for Authorization roles in session/DB
  auto s = manager::instance().find_session(req);

  // Make sure session found or parameters valid. Otherwise, reply login page
  if (s == nullptr || input_params.scope.empty())
    return reply(res, oresp,
                 manager::instance().login_template(),
                 eows::core::http_response::OK);

  // Make sure the specified role is in app
  if (!s->has_role(input_params.scope))
    return forbidden(res, oresp);

  // OK, everything is fine
  return reply(res, oresp,
               manager::instance().authorize_template(),
               eows::core::http_response::OK);
}

void eows::auth::oauth2_authorize::do_post(const eows::core::http_request& req, eows::core::http_response& res)
{
  oauth_parameters input_params(req.query_string());
  authorization_code authorization(input_params);

  auto output_params = authorization.grant(req, res);

  if (output_params.redirect_uri.empty())
  {
    // Retrieve from HTTP Referer
//      const auto headers = req.headers();
//      auto it = headers.find("Referer");
//      if (it != headers.end())
//        output_params.redirect_uri = it->second;
  }

  if (output_params.grant_type == "authorization_code")
  {
    res.set_status(eows::core::http_response::OK);
    const auto json = output_params.to_json();
    return res.write(json.c_str(), json.size());
  }

  res.set_status(eows::core::http_response::moved_permanently);
  const std::string redirect_uri = output_params.redirect_uri;
  output_params.redirect_uri.clear();
  res.add_header(eows::core::http_response::LOCATION, eows::core::to_str(output_params.to_query_string()) + "&redirect_uri="+redirect_uri);
}

void eows::auth::oauth2_info::do_get(const eows::core::http_request& req, eows::core::http_response& res)
{

}

void eows::auth::oauth2_logout::do_get(const eows::core::http_request& req, eows::core::http_response& res)
{

}
