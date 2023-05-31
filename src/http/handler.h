#pragma once

#include "host.h"

#include <httplib.h>

namespace http::handler {
void setup(httplib::Server &srv, const host &host);
}