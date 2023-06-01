#include "handler.h"

#include <string>

#include "Logger.h"
#include "api/sale.h"
#include "responsecodes.h"

#include <httplib.h>

namespace http::handler {
auto logline(const std::string &function) { return "listener::" + function; }

//---------------------------------------------------------------------------
void security(httplib::Server &server, const host &host) {
  const auto fName = logline(static_cast<const char *>(__func__));
  static std::atomic<uint> cid(0);

  server.set_pre_routing_handler([&host, fName](auto &req, auto &res) {
    req.setId(++cid);
    auto ipaddr = req.get_header_value("REMOTE_ADDR");
    auto useragent = req.get_header_value("User-Agent");

    if (host.isBanned(ipaddr)) {
      res.status = code::Unauthorized;
      Logger::getLogger()->warn(fName, "IP Blocked: " + ipaddr, IS_THREAD,
                                req.id());
      return httplib::Server::HandlerResponse::Handled;
    }

    return httplib::Server::HandlerResponse::Unhandled;
  });
}

//---------------------------------------------------------------------------
void errors(httplib::Server &server) {
  const auto fName = logline(static_cast<const char *>(__func__));
  server.set_error_handler([fName](const auto &req, auto &res) {
    auto ipAddr = "IP:" + req.get_header_value("REMOTE_ADDR") + " ";
    auto methodAndPath = req.method + " " + req.path;
    auto status = " " + std::to_string(res.status) + "";
    auto logStr = ipAddr + methodAndPath + status;

    Logger::getLogger()->warn(fName, logStr, IS_THREAD, req.id());
    Logger::getLogger()->warn(fName, req.body, IS_THREAD, req.id());
  });
}

//---------------------------------------------------------------------------
void routing(httplib::Server &server) {
  server.Post("/sale",
              [=](const httplib::Request &req, httplib::Response &res) {
                api::sale::auth(req, res);
              });
  server.Put(R"(^/sale/inc/(\w+)$)",
             [=](const httplib::Request &req, httplib::Response &res) {
               api::sale::inc(req, res);
             });
  server.Get("/sale",
             [=](const httplib::Request &req, httplib::Response &res) -> void {
               api::sale::list(req, res);
             });
}

//---------------------------------------------------------------------------
void setup(httplib::Server &srv, const host &host) {
  security(srv, host);
  errors(srv);
  routing(srv);
}

} // namespace http::handler