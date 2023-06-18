#pragma once

#include <json.hpp>
#include <string>
#include <unordered_set>

using json = nlohmann::json;

namespace http {
struct host {
  std::string ip{};
  uint port;
  std::unordered_set<std::string> bannedips;

  void setup(const json &config) {
    ip = config.at("ip");
    port = config.at("port");
    if (config.contains("bannedIps")) {
      bannedips = config.value("bannedIps", std::unordered_set<std::string>{});
    }
  }

  bool isBanned(const std::string &ipaddr) const {
    return bannedips.find(ipaddr) != bannedips.end();
  }
};

} // namespace http