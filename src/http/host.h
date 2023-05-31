#pragma once

#include <json.hpp>
#include <string>

using json = nlohmann::json;

namespace http {
struct host {
  std::string ip{};
  uint port;

  void setup(const json &config) {
    ip = config.at("ip");
    port = config.at("port");
  }
};

} // namespace http