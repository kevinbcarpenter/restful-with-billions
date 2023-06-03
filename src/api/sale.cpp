#include "sale.h"

#include <iostream>
#include <string>

#include "Logger.h"
#include "data/store.h"
#include "detail/creditcard.h"
#include "httplib.h"
#include "responsecodes.h"
#include "utility.h"

namespace api::sale {
constexpr auto contentType = "application/json";

auto logline(const std::string &func) -> std::string {
  return "api::sale::" + func;
}

//---------------------------------------------------------------------------
void auth(const httplib::Request &req, httplib::Response &res) {
  const auto fName = logline(static_cast<const char *>(__func__));
  Logger *log = Logger::getLogger();

  detail::cardInfo ci = json::parse(req.body);
  ci.guid = utility::guid();
  data::store::save(ci.guid, ci);

  log->save(fName, "Posted Sale", IS_THREAD, req.id());

  res.status = http::code::OK;

  json result{ci};
  res.set_content(result.dump(), contentType);
};

//---------------------------------------------------------------------------
void inc(const httplib::Request &req, httplib::Response &res) {
  const auto fName = logline(static_cast<const char *>(__func__));
  Logger *log = Logger::getLogger();
  json result{};

  detail::cardInfo uci = json::parse(req.body);
  auto guid = req.matches[1];
  auto sci = data::store::get(guid);

  if (sci.has_value()) {
    if (uci.amount != (uci.tipAmount + sci->amount)) {
      log->warn(fName, "New amount with tip does not equal differs.", IS_THREAD,
                req.id());
    }
    sci->tipAmount = uci.tipAmount;
    sci->amount = uci.amount;

    log->save(fName, "Incremented Sale", IS_THREAD, req.id());
    res.status = http::code::OK;
    json result = sci.value();
  } else {
    log->warn(fName, "Transaction not found.", IS_THREAD, req.id());
    res.status = http::code::InternalServerError;
  }

  res.set_content(result.dump(), contentType);
};

//---------------------------------------------------------------------------
void list(const httplib::Request &req, httplib::Response &res) {
  const auto fName = logline(static_cast<const char *>(__func__));
  Logger *log = Logger::getLogger();
  json result{};

  result = data::store::list();

  log->save(fName, "Listed Sales", IS_THREAD, req.id());
  res.status = http::code::OK;
  res.set_content(result.dump(), contentType);
}

//---------------------------------------------------------------------------
void reverse(const httplib::Request &req, httplib::Response &res) {
  const auto fName = logline(static_cast<const char *>(__func__));
  Logger *log = Logger::getLogger();
  json result{};

  // detail::cardInfo uci = json::parse(req.body);
  auto guid = req.matches[1];
  auto sci = data::store::get(guid);

  if (sci.has_value()) {
    data::store::remove(guid);

    log->save(fName, "Reversed/Removed Sale", IS_THREAD, req.id());
    res.status = http::code::OK;
    json result = sci.value();
  } else {
    log->warn(fName, "Transaction not found to reverse.", IS_THREAD, req.id());
    res.status = http::code::InternalServerError;
  }

  res.set_content(result.dump(), contentType);
};

} // namespace api::sale