#include <iostream>
#include <httplib.h>

int main(int /* argc*/, char ** /*argv[]*/) {

// HTTP
httplib::Client cli("http://www.google.com");

auto res = cli.Get("/");
std::cout << "Status: " << res->status << std::endl;
std::cout << "Body: " << res->body << std::endl;

return 0;
}