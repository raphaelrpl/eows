#include "routes.hpp"
#include "manager.hpp"

// EOWS OAuth
#include "oauth2/data_types.hpp"
#include "oauth2/authorization_code.hpp"

// EOWS Core
#include "../core/utils.hpp"
#include "../core/http_request.hpp"
#include "../core/http_response.hpp"

void eows::auth::oauth2_authorize::do_get(const eows::core::http_request& req, eows::core::http_response& res)
{
  oauth_parameters input_params(req.query_string());

  eows::core::http_response::status_t status_code;

  if (input_params.error.empty() &&  (input_params.response_type.empty() || input_params.client_id.empty()))
    status_code = eows::core::http_response::forbidden;
  else
  {
    if (!input_params.scope.empty())
    {
      // Check for Authorization roles in session/DB
      auto s = manager::instance().find_session(req);

      if (s != nullptr)
      {
        if (!s->expired() && s->has_role(input_params.scope))
        {
          // OK, everything is fine.
        }
      }
    }
  }

  res.set_status(status_code);
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
