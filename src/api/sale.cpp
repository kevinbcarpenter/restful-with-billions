#include "sale.h"

#include <iostream>
#include <string>

#include "Logger.h"
#include "httplib.h"

namespace api::sale {

auto logInfo(const std::string &func) -> std::string {
  return "api::sale::" + func;
}

auto auth(const httplib::Request &req, httplib::Response /*&res*/) -> json {
  const auto fName = logInfo(static_cast<const char *>(__func__));
  Logger *log = Logger::getLogger();

  log->save(fName, "Posted Sale", IS_THREAD, req.id());
  return req.body;
};
} // namespace api::sale