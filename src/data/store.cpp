#include "store.h"
#include "Logger.h"
#include "detail/creditcard.h"
#include <iterator>

namespace data {

auto logline(const std::string &func) { return "data::store::" + func; }

//---------------------------------------------------------------------------
void store::save(const std::string &guid, detail::cardInfo ci) {
  const auto fName = logline(static_cast<const char *>(__func__));
  std::lock_guard<std::mutex> lock(transactMutex);

  transactions.insert_or_assign(guid, ci);

  Logger::getLogger()->save(fName, "Stored " + guid, IS_THREAD);
  Logger::getLogger()->save(fName, "Total Count: " + countstr(), IS_THREAD);
}

//---------------------------------------------------------------------------
auto store::get(const std::string &guid) -> std::optional<detail::cardInfo> {
  const auto fName = logline(static_cast<const char *>(__func__));
  std::lock_guard<std::mutex> lock(transactMutex);

  const auto t = transactions.find(guid);
  if (t == transactions.end()) {
    return std::nullopt;
  }

  Logger::getLogger()->save(fName, "Retrieved " + guid, IS_THREAD);
  Logger::getLogger()->save(fName, "Total Count: " + countstr(), IS_THREAD);

  return {t->second};
}

//---------------------------------------------------------------------------
auto store::list() -> std::vector<detail::cardInfo> {
  const auto fName = logline(static_cast<const char *>(__func__));
  std::vector<detail::cardInfo> l;
  l.reserve(count());

  std::lock_guard<std::mutex> lock(transactMutex);
  std::transform(transactions.begin(), transactions.end(),
                 std::back_insert_iterator(l),
                 [](std::pair<std::string, detail::cardInfo> const &pair) {
                   return pair.second;
                 });

  return l;
}

//---------------------------------------------------------------------------
void store::remove(const std::string &guid) {
  const auto fName = logline(static_cast<const char *>(__func__));
  std::lock_guard<std::mutex> lock(transactMutex);
  auto t = transactions.find(guid);
  if (t != transactions.end()) {
    transactions.erase(t);
  }

  Logger::getLogger()->save(fName, "Removed " + guid, IS_THREAD);
  Logger::getLogger()->save(fName, "Total Count: " + countstr(), IS_THREAD);
}

} // namespace data