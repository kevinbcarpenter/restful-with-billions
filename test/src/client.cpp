#include <iostream>
#include <httplib.h>

int main(int /* argc*/, char ** /*argv[]*/) {

// HTTP
httplib::Client cli("http://127.0.0.1:8080");

auto res = cli.Get("/hi");
std::cout << "Status: " << res->status << std::endl;
std::cout << "Body: " << res->body << std::endl;

return 0;
}