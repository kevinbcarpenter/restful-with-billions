#include "restful.h"

#include <exception>
#include <memory>
#include <string>
#include <thread>

namespace {
constexpr const auto VERSION{"1.0.0"};
constexpr const auto APP_NAME{"rest-server"};
// constexpr const auto TITLE{APP_NAME + " - " + VERSION};

// auto logInfo(const std::string &func) { return "Restufl: " + func; }

} // namespace

Restful::Restful(std::string appName) : App(appName) {
  // nothing for now.
}

void Restful::start() {
  std::cout << "Starting server on port 8080" << std::endl;
  // svr.listen("0.0.0.0", 8080);
}

// void Restful::setup(const json &config) {
//   try {

//   } catch (const std::exception &ex) {
//     std::cout << "Failure during config: " << ex.what() << std::endl;
//   }
// }

void Restful::run() { App::run(); }

int main(int argc, char *argv[]) {
  App::main(argc, argv, VERSION, new Restful(APP_NAME));
  return 0;
}