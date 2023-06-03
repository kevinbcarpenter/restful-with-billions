#include <httplib.h>
#include <iostream>
#include <sstream>

#include "detail/creditcard.h"

#include <json.hpp>

using json = nlohmann::json;

constexpr auto contentType = "application/json";
constexpr auto TESTCOUNT = 5;

//---------------------------------------------------------------------------
auto testPost(httplib::Client &cli, detail::cardInfo &ci) -> detail::cardInfo {
  json payload = ci;
  auto res = cli.Post("/sale", payload.dump(), contentType);

  detail::cardInfo rci = json::parse(res->body)[0];
  std::cout << "Status: " << res->status << std::endl;
  std::cout << "Guid: " << rci.guid << std::endl;
  std::cout << "Body: " << res->body << std::endl;

  return rci;
}

//---------------------------------------------------------------------------
void testPut(httplib::Client &cli, detail::cardInfo &ci) {
  ci.tipAmount = ci.amount * .20;
  ci.amount += ci.tipAmount;
  json payload = ci;

  std::stringstream putPath{};
  putPath << "/sale/inc/" << ci.guid;

  auto res = cli.Put(putPath.str(), payload.dump(), contentType);

  std::cout << "Status: " << res->status << std::endl;
  std::cout << "Body: " << res->body << std::endl;
}

//---------------------------------------------------------------------------
auto testGet(httplib::Client &cli) -> std::vector<detail::cardInfo> {
  std::cout << "RESULT from GET" << std::endl;
  auto res = cli.Get("/sale");
  // std::cout << "Status: " << res->status << std::endl;
  // std::cout << "Body: " << res->body << std::endl;
  auto trans = json::parse(res->body);

  return trans;
}

//---------------------------------------------------------------------------
auto testRemove(httplib::Client &cli, std::string guid) {
  std::stringstream delPath{};
  delPath << "/sale/void/" << guid;

  auto res = cli.Delete(delPath.str());

  return res;
}

//---------------------------------------------------------------------------
auto main(int /* argc*/, char ** /*argv[]*/) -> int {
  httplib::Client cli("http://127.0.0.1:8080");

  for (int i = 0; i < TESTCOUNT; ++i) {
    detail::cardInfo c1{"4111111111111111", "0524", 125.00, 0, 1, 1, ""};
    auto c2 = testPost(cli, c1);
    testPut(cli, c2);
  }

  auto sales = testGet(cli);
  for (auto s : sales) {
    auto d1 = testRemove(cli, s.guid);
  }

  return 0;
}