# OAuth 2.0

## Overview

The OAuth2 specification is a flexible framework standard that describes a set of grants for a client app to acquire an access token for API request authorization endpoint to protected resources. **OAuth 2.0 is not an authentication protocol**. While **Authentication** is a process to identify users and their informations, the **Authorization** acts for rule permissions which a approval is required to manipulate partial of user information.

This OAuth 2.0 spec defines four roles:

- **Resource owner (User)** - Entity capable of granting access to some portion of their account;
- **Resource Server (API Server)** - Server that provides protected resources, capable of accepting, validating and responding request to protect resources using *access_tokens*;
- **Client** - Application making protected resource requests on behalf of the resource owner and with its authorization:
  * **Public** - Represents clients that are cannot maintain the confidentiality of a *client_secret*. In other words, the secret is not used for these apps (Mobile Apps and Javascripts apps).

  * **Confidential** - Represents clients which have hability to maintain the confidentiality of the *client_secret*. Typically, they are applications that the source code is not accessible to users. (web-apps)

- **Authorization Server** - Server handler, issuing *access tokens* to the client after successfully authenticating the resource owner. The user approves or denies the request.

The OAuth2 concepts has multiple grant type implementations:

- *Authorization code grant*
- *Implicit Grant*
- *Resource Owner password Credentials Grant*
- *Client Credentials Grant*

**You don't need to implement all of these concepts**. In our scenario, both **Authorization code grant** and **Implicit Grant** works properly. We should be able to display them while creating a new application.

The main token type of OAuth2 standard is *Bearer tokens*. 

The [Spec RFC 6750](https://tools.ietf.org/html/rfc6750#section-1.2) defines *Bearer Token* as:
> A security token with the property that any party in possession ofthe token (a "bearer") can use the token in any way that any other party in possession of it can.  Using a bearer token does not require a bearer to prove possession of cryptographic key material (proof-of-possession).

In other words, when a user authenticates an client application, the authorization server generates a *Bearer Token* that can be used to get an access token.

**Notes** 

- *Bearer Token* should not be passed in page URLS. Instead of that, prefer to pass through HTTP Header (e.g *X-ESENSING-EOWS-TOKEN: BearerTokenHere*)

- *OAuth2 Server* must run only over HTTPS protocol support.

## Glossary

- **App** - or *client* represents an interface for accessing user resources.

- **Redirect URIs** - The OAuth2 Server will only redirect users to a **registered URI**. Any redirect URI must be protected with TLS (HTTPs support) to prevent *access_token* to be intercepted during request redirect.

- **Client ID** - An unique identifier for an **App**. Client ID is public information and used to build login URL or included into web-page or client requesters.

- **Client Secret** - An unique value for **App** using **Client ID**. The *secret* must be kept confidential.

- **Roles** - Defines which roles (or protected resources) will be provided to access.

## Comparison with OAuth 1.0

OAuth 1.0 does not offered a safe way requirements of signing requests with the client ID and secret. OAuth 2 recognizes this difficulty and replaces signatures requiring **HTTPS** for all communications between browsers, clients and the API.

This spec also requires that the API server has access to the application's ID and secret. In other words, the Authorization server and Resource server are the same application. It is of course breaks the architecture of most large providers where the both are completely separated. 

OAuth 2 supports the separation of the roles of obtaining user authorization and handling API calls. Larger providers needing this scalability are free to implement it as such, and smaller providers can use the same server for both roles if they wish.


## Grant Types

### Authorization code grant

The client will request a code by intermediation of the user. Once with code, the client will be able to request access token to access user's resources. This grant type is recommended for apps running on a web server, browser-based and mobile apps.

```
+----------+
| Resource |
|   Owner  |
|          |
+----------+
     ^
     |
    (B)
+----|-----+          Client Identifier      +---------------+
|          +----(A)-- & Redirection URI ---->|               |
|  User-   |                                 | Authorization |
|  Agent   +----(B)-- User authenticates --->|     Server    |
|          |                                 |               |
|          +----(C)-- Authorization Code ---<|               |
+----------+                                 +---------------+
  |    |                                         ^      v
 (A)  (C)                                        |      |
  |    |                                         |      |
  ^    v                                         |      |
+---------+                                      |      |
|         |>---(D)-- Authorization Code ---------'      |
|  Client |          & Redirection URI                  |
|         |                                             |
|         |<---(E)----- Access Token -------------------'
+---------+       (w/ Optional Refresh Token)
```
**Representation from [OAuthLib Docs](http://oauthlib.readthedocs.io/en/latest/oauth2/grants/authcode.html)**

#### Workflow

The client will redirect the user to authorization server with following parameters:

```
{
  "response_type": "code", // Fixed to "code"
  "client_id": string,     // Client Identifier
  "redirect_uri": string,  // Client Redirect URI. Optional. by default redirect yo pre-registered URI
  "scope": string,         // Space delimited list of scopes
  "state": string          // CSRF token for security reasons. You should store it in session and validate when they return. (Optional)
}
```

All of these parameters will be validated on authorization server. The server may ask user to log in and approve client work flow. If approved, they will be redirected from authorization server back to client with following parameters:

```
{
  "code": string, // With authorization code
  "state": string // With original state parameters sent in request.
}
```

For authorize new functionality, the client will send **POST** request to the authorization server with following parameters:

```
{
  "grant_type": "authorization_code", // Fixed to "authorization_code"
  "client_id": string,     // Client Identifier
  "client_secret": string, // Space delimited list of scopes
  "redirect_uri": string,  // Client Redirect URI.
  "code": string           // Authorization code received from query string
}
```

**Note** that **client_secret** is sent since this request is made from server-side code.

The authorization server will respond:

```
{
  "token_type": "Bearer", // Fixed to "Bearer"
  "expires_in": integer,  // Time expiration in seconds
  "access_token": string, // Temporary access token.
  "refresh_token": string // Refresh token used to acquire new access token when expires.
}
```

### Implicit grant

The implicit grant is a simplified authorization code flow optimized for clients implemented in a browser using a scripting language such as JavaScript. In the implicit flow, instead of issuing the client an authorization code, the client is issued an access token directly.

Unlike the authorization code grant, the client receives the access token as the result of authorization request. In the implicit flow all data is volatile and there's nothing stored in the app. 

Note that the implicit grant flow is usually used in scenarios where storing client id and client secret is not recommended (e.g device) due people may have access to the client code and therefore could get the credentials and pretend to become resource clients, as described by disclaimer.

```
+----------+
| Resource |
|  Owner   |
+----------+
     ^
     |
    (B)
+----------+          Client Identifier     +---------------+
|          |----(A)-- & Redirection URI --->|               |
|  User-   |                                | Authorization |
|  Agent   |----(B)-- User authenticates -->|     Server    |
|          |                                |               |
|          |<---(C)--- Redirection URI ----<|               |
|          |          with Access Token     +---------------+
|          |            in Fragment
|          |                                +---------------+
|          |----(D)--- Redirection URI ---->|   Web-Hosted  |
|          |          without Fragment      |     Client    |
|          |                                |    Resource   |
|     (F)  |<---(E)------- Script ---------<|               |
|          |                                +---------------+
+----------+
  ^    |
  |    |
 (A)  (G) Access Token
  |    |
  |    v
+--------+
| Client |
+--------+
```
**Representation from [OAuthLib Docs](http://oauthlib.readthedocs.io/en/latest/oauth2/grants/implicit.html)**

#### Workflow

The client will redirect the user to authorization server with following parameters:

```
{
  "response_type": "token", // Fixed to "token"
  "client_id": string,     // Client Identifier
  "redirect_uri": string,  // Client Redirect URI. Optional. by default redirect yo pre-registered URIe 
  "scope": string,         // Space delimited list of scopes
  "state": string          // CSRF token for security reasons. You should store it in session and validate when they return. (Optional)
}
```

All of these parameters will be validated on authorization server. The server may ask user to log in and approve client work flow. If approved, they will be redirected from authorization server back to client with following parameters:

```
{
  "token_type": "Bearer",
  "expires_in": integer, // Time expiration
  "access_token": string, // Access token
  "state": string // With original state parameters sent in request.
}
```

**Note** this grant does not return refresh token.

### Resource Owner password Credentials Grant

With this grant type, the user provides their credentials directly to the service application, which uses the credentials to obtain an access token from the service. This grant is for trusted first party clients both on the web and in native device applications. 

**Note** *this grant type should only be enabled on the authorization server if other flows are not viable. Also, it should only be used if the application is trusted by the user (e.g. it is owned by the service, or the user's desktop OS). [2]*

## Authorization Interface

Once user authenticates in EOWS platform, the Authorization server should redirect either to the Authorize interface (if it is not allowed yet) or to provided redirect URI (if it is valid URI in app).

TODO: Display Authorize Page

The interface must display what is being accessed from user account:
  - You may provide checkboxes of permissions and let user decide if specific role is allowed to be accessed. If provided, you must
    send a parameters for revoking the specified roles.

TODO: DISPLAY IMAGE

## Token Introspection Endpoint

Whenever an client make a request to the resource server, the resource server needs to verify the access token. Although OAuth 2.0 spec does not define a specific method of how the resource server should verify access tokens, there is another spec called OAuth 2.0 Token Introspection that provides a protocol for return user information through Token originality. In this way, we must define an pattern for token background validity using OAuth 2.0 Token Introspection template to trust in client requests.

The OAuth 2.0 Token Introspection spec has several attributes that may be implemented. The following attributes are essentials to guarantee token originality:

```
{
  "active":    bool,
  "scope":     string,
  "client_id": string,
  "username":  string,
  "exp":       number
}
```

- **active** - *Required*. Boolean value for token current active. The value should be “true” if the token has been issued by this authorization                    server, has not been revoked by the user, and has not expired;

- **scope** - *Optional*. Contains list of scopes associated with this *access_token*;

- **client_id** - *Optional* Client Identifier;

- **username** - *Optional* User associated with this *access_token*;

- **exp** - *Optional*. Unix timestamp indicating when *access_token* will expire.

The following libraries are implemented in C++11 using JWT Tokens (Spec that implements OAuth 2.0 Token Introspection)
- [**jwtxx**](https://github.com/madf/jwtxx)
- [**Jwt-cpp**](https://github.com/pokowaka/jwt-cpp)


## Implementation

### Configuration

All configurations are defined in `share/eows/config/auth.json`
```
{
  "session_expiration": integer,
  "oauth2_code_expiration": integer,
  "oauth2_authorize_uri": string,
  "oauth2_logout_uri": string,
  "oauth2_info_uri": string,
  "oauth2_login_template_path": string,
  "oauth2_message_template_path": string,
  "oauth2_error_template_path": string,
  "use_refresh_token": bool
}
```

Description:

  * **session_expiration** - Defines a session timeout expiration in seconds. Default is a day (86400 seconds).

  * **oauth2_code_expiration** - Defines a max lifetime of code (in seconds) used to exchange for an access_token. Default is 90.

  * **oauth2_authorize_uri** - Defines subaddress of oauth handlers. Usually are "auth" or "authorize".
  
  * **oauth2_logout_uri** - Sub address to logout users from authorization server

  * **oauth2_info_uri** - Sub address to obtain information about a user authorized clients

  * **oauth2_login_template_path** - Path to the file container HTML template to authorize clients for users that are not logged in. (Relative to data dir)

    - **Example**: TODO. We should define a template with tag elements to replace in server side.

    ```html

    <!-- www/oauth2/login.html -->

    <!-- CSS elements/includes -->

    <form id="authorization-form" method="POST" action="/oauth2/" enctype="application/x-www-form-urlencoded">
      <input type="text" name="username" placeholder="username"><br/>
      <input type="password" name="password" placeholder="password"><br/>
      <input type="hidden" name="response_type" value="{{response_type}}">
      <input type="hidden" name="scope" value="{{scope}}">
      <input type="hidden" name="client_id" value="{{client_id}}">
      <input type="hidden" name="redirect_uri" value="{{redirect_uri}}">
      <input type="hidden" name="state" value="{{state}}">
      <button>Sign In</button>
    </form>
    ```
  
  * **oauth2_message_template_path** - Path to the file containing the HTML template to authorize clients for users that are already logged. (It must not show username and password fields, but an authorize button with client meta information)
  
    - **Example** Same in **oauth2_login_template_path** without **username** and **password** fields

  * **oauth2_error_template_path** - Path to the file containing the HTML template to show when users try to access oauth2 with wrong URI.

  * **use_refresh_token** - Bool value used to ask for code grant type with refresh_token.


### API Structure


* **Users**

  **This API is not implemented yet**

* **Workspaces**

  **This API is not implemented yet**

* **WTSS**

  **This API is not implemented yet**

* **WCS**

  **This API is not implemented yet**

* **WTSPS**

  **This API is not implemented yet**


### Scopes (Roles)

| Name | Description |
|---|---|
| `user.email` | Allow to read user information |
|  |  |

## References

- **1** - [OAuth 2.0](https://oauth.net/2/)
- **2** - [RFC 6749 - The OAuth 2.0 Authorization Framework](https://tools.ietf.org/html/rfc6749)
- **3** - [Digital Ocean - An Introduction to OAuth 2](https://www.digitalocean.com/community/tutorials/an-introduction-to-oauth-2)
- **4** - [Guide to OAuth2 Grants](https://alexbilbie.com/guide-to-oauth-2-grants/)
- **5** - [C++ Rest SDK OAuth2 Server - Granada](https://cookinapps.io/2016/06/c++-rest-sdk-oauth-2-0-server/)
- **6** - [OAuth2 Servers](https://www.oauth.com/oauth2-servers/)
- **7** - [User Authentication with OAuth2.0](https://oauth.net/articles/authentication/)
- **8** - [OAuth2 Token Introspection](https://tools.ietf.org/html/draft-ietf-oauth-introspection-11)