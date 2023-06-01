#include "creditcard.h"

namespace detail {
void from_json(const json &j, cardInfo &c) {
  c.account = j.value("account", "");
  c.expiration = j.value("expiration", "");
  c.guid = j.value("guid", "");
  c.amount = j.value("amount", 0.0);
  c.tipAmount = j.value("tipAmount", 0.0);
  c.batchId = j.value("batchId", 0);
  c.transaction = j.value("transaction", 0);
}

void to_json(json &j, const cardInfo &c) {
  j = json{{"account", c.account},
           {"expiration", c.expiration},
           {"guid", c.guid},
           {"amount", c.amount},
           {"tipAmount", c.tipAmount},
           {"batchId", c.batchId},
           {"transaction", c.transaction}};
};

} // namespace detail