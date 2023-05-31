#include "listener.h"
#include "httplib.h"

namespace http::listener {
    void run(host host) {
        httplib::Server srv;
        srv.listen(host.ip, host.port);
    }
}