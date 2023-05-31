#pragma once

#include "json.hpp"
#include <string>

using json = nlohmann::json;

// {
//   "account" : "4111111111111111",
//   "expiration" : "0524",
//   "amount" : 127.99,
//   "batchID" : 444,
//   "transaction" : 123
// }

namespace detail {

struct cardInfo {
  std::string account{};
  std::string expiration{};
  double amount{0.0};
  uint batchId{0};
  uint transaction{0};
};

void from_json(const json &j, cardInfo &c);
void to_json(json &j, const cardInfo &c);

} // namespace detail