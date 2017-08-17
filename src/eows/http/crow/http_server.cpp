// EOWS
#include "http_server.hpp"
#include "../../exception.hpp"
#include "../../core/app_settings.hpp"
#include "../../core/defines.hpp"
#include "../../core/logger.hpp"
#include "../../core/service_operations_manager.hpp"
#include "../../core/utils.hpp"
#include "../../core/web_service_handler.hpp"
#include "http_request.hpp"
#include "http_response.hpp"

// STL
#include <cstdlib>

// Boost
#include <boost/filesystem.hpp>
//#include <boost/format.hpp>

// Crow
#include <crow_all.h>

class LogHandler : public ::crow::ILogHandler
{
  public:

    void log(std::string /*message*/, crow::LogLevel /*level*/) override
    {
      // std::cerr << message;
    }
};

struct ws_handler
{
  eows::core::web_service_handler* h_;
  
  ws_handler(eows::core::web_service_handler* h)
    : h_(h)
  {
  }
  
  void operator()(const crow::request& req, crow::response& res)
  {
    eows::http::crow::http_request req_wrapper(req);
    eows::http::crow::http_response res_wrapper(res);

    switch(req.method)
    {
      case crow::HTTPMethod::GET:
        h_->do_get(req_wrapper, res_wrapper);
        break;

      case crow::HTTPMethod::POST:
        h_->do_post(req_wrapper, res_wrapper);
        break;

      default:
        break;
    }


    res.end();
  }
};

static void prepare_routes(crow::SimpleApp& app)
{
  std::vector<std::string> routes = eows::core::service_operations_manager::instance().registered_paths();
  
  for(std::string r : routes)
  {
    ws_handler h(eows::core::service_operations_manager::instance().get(r));

    app.route_dynamic(std::move(r)).methods(crow::HTTPMethod::GET, crow::HTTPMethod::POST)(h);
  }
}

struct crow_info_t
{
  std::string listen_address;
  uint16_t listening_port;
  uint16_t threads;
  bool ssl_enabled = false;
  std::string ssl_certificate;
  std::string ssl_key;
};
      
static crow_info_t load_config()
{
  const rapidjson::Document& doc = eows::core::app_settings::instance().get();
        
  if(!doc.HasMember("crow"))
    throw eows::parse_error("Please check key 'crow' in file '" EOWS_CONFIG_FILE "'.");
  
  const rapidjson::Value& jcrow = doc["crow"];
      
  if(!jcrow.IsObject())
    throw eows::parse_error("Key 'crow' must be a valid JSON object in file '" EOWS_CONFIG_FILE "'.");
        
  rapidjson::Value::ConstMemberIterator jlisten_address = jcrow.FindMember("listen_address");

  if((jlisten_address == jcrow.MemberEnd()) || (!jlisten_address->value.IsString()))
    throw eows::parse_error("Please check key 'listen_address' in file 'crow.json'.");

  crow_info_t app_cfg;
  
  app_cfg.listen_address = jlisten_address->value.GetString();
  
  rapidjson::Value::ConstMemberIterator jlistening_port = jcrow.FindMember("listening_port");

  if((jlistening_port == jcrow.MemberEnd()) || (!jlistening_port->value.IsUint()))
    throw eows::parse_error("Please check key 'listening_port' in file 'crow.json'.");

  app_cfg.listening_port = static_cast<uint16_t>(jlistening_port->value.GetUint());
        
  rapidjson::Value::ConstMemberIterator jthreads = jcrow.FindMember("threads");

  if((jthreads == jcrow.MemberEnd()) || (!jthreads->value.IsUint()))
    throw eows::parse_error("Please check key 'threads' in file 'crow.json'.");

  app_cfg.threads = static_cast<uint16_t>(jthreads->value.GetUint());

  rapidjson::Value::ConstMemberIterator ssl_it = jcrow.FindMember("ssl");

  // If Found
  if (ssl_it != jcrow.MemberEnd())
  {
    rapidjson::Value::ConstMemberIterator ssl_enabled_it = ssl_it->value.FindMember("enabled");

    if ((ssl_enabled_it != ssl_it->value.MemberEnd()) && (ssl_enabled_it->value.GetBool()))
    {
      app_cfg.ssl_certificate = eows::core::read_node_as_string(ssl_it->value, "certificate");
      app_cfg.ssl_key = eows::core::read_node_as_string(ssl_it->value, "key");
      app_cfg.ssl_enabled = true;
    }
  }

  return app_cfg;
}

int
eows::http::crow::http_server::run()
{
  EOWS_LOG_INFO("Prepararing web server...");

  crow_info_t cfg_info = load_config();
  
  ::crow::SimpleApp app;
  
  prepare_routes(app);

  LogHandler lh;

  ::crow::logger::setLogLevel(::crow::LogLevel::INFO);
  ::crow::logger::setHandler(&lh);

  if (cfg_info.ssl_enabled)
  {
    EOWS_LOG_INFO("Configuring HTTPS server...");
    app.ssl_file(cfg_info.ssl_certificate, cfg_info.ssl_key);
  }
  
  app.bindaddr(cfg_info.listen_address)
     .port(cfg_info.listening_port)
     .multithreaded()
     .concurrency(cfg_info.threads)
     .run();
  
  
  return EXIT_SUCCESS;
}

