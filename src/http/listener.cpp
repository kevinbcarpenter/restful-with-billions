#include "listener.h"
#include "handler.h"
#include "httplib.h"

namespace http::listener {
void run(host host) {
  httplib::Server srv;
  handler::setup(srv, host);
  srv.listen(host.ip, host.port);
}
} // namespace http::listener