#pragma once

#include "json.hpp"
#include <string>

using json = nlohmann::json;

namespace detail {

struct cardInfo {
  std::string account{};
  std::string expiration{};
  std::string guid{};
  double amount{0.0};
  double tipAmount{0.0};
  uint batchId{0};
  uint transaction{0};
};

void from_json(const json &j, cardInfo &c);
void to_json(json &j, const cardInfo &c);

} // namespace detail