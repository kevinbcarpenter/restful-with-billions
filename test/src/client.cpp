#include <httplib.h>
#include <iostream>
#include <sstream>
#include <thread>

#include "detail/creditcard.h"

#include <json.hpp>

using json = nlohmann::json;

constexpr auto TESTCOUNT = 5;
constexpr auto DELAY = 1;
constexpr auto TIP_PCT = .2;
constexpr auto contentType = "application/json";

//---------------------------------------------------------------------------
auto testPost(httplib::Client &cli, detail::cardInfo &ci) -> detail::cardInfo {
  json payload = ci;
  if (auto res = cli.Post("/sale", payload.dump(), contentType)) {
    detail::cardInfo rci = json::parse(res->body)[0];
    std::cout << "POST/CREATE - Status: " << res->status << std::endl
              << "Guid: " << rci.guid << std::endl
              << "Body: " << res->body << std::endl;

    return rci;
  } else {
    std::cerr << "error: httpclient: testPost: " << res.error() << '\n';
    return {};
  }
}

//---------------------------------------------------------------------------
void testPut(httplib::Client &cli, detail::cardInfo &ci) {
  ci.tipAmount = ci.amount * TIP_PCT;
  ci.amount += ci.tipAmount;
  json payload = ci;

  std::stringstream putPath{};
  putPath << "/sale/inc/" << ci.guid;

  if (auto res = cli.Put(putPath.str(), payload.dump(), contentType)) {
    std::cout << "PUT/UPDATE - Status: " << res->status << std::endl;
  } else {
    std::cerr << "error: httpclient: testPut: " << res.error() << '\n';
  }
}

//---------------------------------------------------------------------------
auto testGet(httplib::Client &cli) -> std::vector<detail::cardInfo> {
  if (auto res = cli.Get("/sale")) {
    std::cout << "RESULT from GET" << std::endl;
    std::cout << "GET/LIST - Status: " << res->status << std::endl
              << "Body: " << res->body << std::endl;
    auto trans = json::parse(res->body);
    return trans;
  } else {
    std::cerr << "error: httpclient: testGet: " << res.error() << '\n';
    return {};
  }
}

//---------------------------------------------------------------------------
auto testRemove(httplib::Client &cli, const std::string &guid) {
  std::stringstream delPath{};
  delPath << "/sale/void/" << guid;

  if (auto res = cli.Delete(delPath.str())) {
    std::cout << "DELETE/DELETE - Status: " << res->status << std::endl;
    return res;

  } else {
    std::cerr << "error: httpclient: testRemove: " << res.error() << '\n';
    return res;
  }
}

//---------------------------------------------------------------------------
auto main(int /* argc*/, char ** /*argv[]*/) -> int {
  uint idleTime = DELAY;
  httplib::Client cli("http://127.0.0.1:8080");

  for (int i = 0; i < TESTCOUNT; ++i) {
    detail::cardInfo c1{"4111111111111111", "0524", 125.00, 0, 1, 1, ""};
    auto c2 = testPost(cli, c1);
    testPut(cli, c2);
    std::this_thread::sleep_for(std::chrono::seconds(idleTime));
  }

  auto sales = testGet(cli);
  for (auto s : sales) {
    auto d1 = testRemove(cli, s.guid);
    std::this_thread::sleep_for(std::chrono::seconds(idleTime));
  }

  return 0;
}
