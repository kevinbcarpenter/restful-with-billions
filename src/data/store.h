#pragma once

#include <iostream>
#include <map>
#include <mutex>
#include <sstream>
#include <string>

#include "detail/creditcard.h"

#include "json.hpp"

using json = nlohmann::json;

namespace data {

// ---------------------------------------------------------------------------
class store {
public:
  static void save(const std::string &guid, detail::cardInfo ci);
  static auto get(const std::string &guid) -> std::optional<detail::cardInfo>;
  static auto list() -> std::vector<detail::cardInfo>;
  static void remove(const std::string &guid);
  static auto count() -> size_t { return transactions.size(); }
  static auto countstr() -> std::string {
    std::stringstream ss;
    ss << count();
    return ss.str();
  }

private:
  static inline std::mutex transactMutex;
  static inline std::map<std::string, detail::cardInfo> transactions;
};

} // namespace data