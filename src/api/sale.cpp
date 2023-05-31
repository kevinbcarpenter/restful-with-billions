#include "sale.h"

#include <iostream>
#include <string>

#include "Logger.h"
#include "detail/creditcard.h"
#include "httplib.h"
#include "responsecodes.h"

namespace api::sale {
constexpr auto contentType = "application/json";

auto logInfo(const std::string &func) -> std::string {
  return "api::sale::" + func;
}

void auth(const httplib::Request &req, httplib::Response &res) {
  const auto fName = logInfo(static_cast<const char *>(__func__));
  Logger *log = Logger::getLogger();

  detail::cardInfo ci = json::parse(req.body);

  log->save(fName, "Posted Sale", IS_THREAD, req.id());

  res.status = http::code::OK;

  json result{ci};
  res.set_content(result.dump(), contentType);
};
} // namespace api::sale