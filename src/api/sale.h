#pragma once

#include "httplib.h"

#include "json.hpp"

using json = nlohmann::json;

namespace api::sale {

void auth(const httplib::Request &req, httplib::Response &res);
void inc(const httplib::Request &req, httplib::Response &res);
void list(const httplib::Request &req, httplib::Response &res);
void reverse(const httplib::Request &req, httplib::Response &res);

} // namespace api::sale