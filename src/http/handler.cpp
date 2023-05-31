#include "handler.h"

#include <string>

#include "Logger.h"
#include "api/sale.h"
#include "responsecodes.h"

#include <httplib.h>

namespace http::handler {

//---------------------------------------------------------------------------
void security(httplib::Server &server, const host &host) {
  static std::atomic<uint> cid(0);

  server.set_pre_routing_handler([&host](auto &req, auto &res) {
    req.setId(++cid);
    auto ipaddr = req.get_header_value("REMOTE_ADDR");
    auto useragent = req.get_header_value("User-Agent");

    if (host.isBanned(ipaddr)) {
      res.status = code::Unauthorized;
      Logger::getLogger()->warn(static_cast<const char *>(__func__),
                                "IP Blocked: " + ipaddr, IS_THREAD, req.id());
      return httplib::Server::HandlerResponse::Handled;
    }

    return httplib::Server::HandlerResponse::Unhandled;
  });
}

//---------------------------------------------------------------------------
void errors(httplib::Server &server) {
  server.set_error_handler([](const auto &req, auto &res) {
    auto ipAddr = "IP:" + req.get_header_value("REMOTE_ADDR") + " ";
    auto methodAndPath = req.method + " " + req.path;
    auto status = " " + std::to_string(res.status) + "";
    auto logStr = ipAddr + methodAndPath + status;

    Logger::getLogger()->warn(static_cast<const char *>(__func__), logStr,
                              IS_THREAD, req.id());
  });
}

//---------------------------------------------------------------------------
void routing(httplib::Server &server) {
  server.Post("/sale",
              [=](const httplib::Request &req, httplib::Response &res) {
                api::sale::auth(req, res);
              });
}

//---------------------------------------------------------------------------
void setup(httplib::Server &server, const host &host) {
  security(server, host);
  errors(server);
  routing(server);
}

} // namespace http::handler