#include "restful.h"
#include "http/host.h"
#include "http/listener.h"
#include <Logger.h>

#include <exception>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <thread>

namespace {
const std::string VERSION{"0.41"};
const std::string APP_NAME{"restful"};

auto logline(const std::string &func) { return "Restful: " + func; }

} // namespace

//---------------------------------------------------------------------------
Restful::Restful(std::string appName) : App(appName) {
  // nothing for now.
}

//---------------------------------------------------------------------------
void Restful::start() {
  const auto fname = ::logline(static_cast<const char *>(__func__));

  http::host host;
  host.setup(App::getConfig().at("httpServer"));

  std::ostringstream os;
  os << "Starting server v" << VERSION << " on " << host.ip << ":" << host.port
     << std::endl;

  std::thread(&http::listener::run, std::move(host)).detach();

  Logger::getLogger()->save(fname, os.str(), IS_MAIN);
}

//---------------------------------------------------------------------------
void Restful::idle() {
  const auto fname = ::logline(static_cast<const char *>(__func__));
  constexpr int idleTime = 5;
  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(idleTime));
    Logger::getLogger()->save(fname, "idle...", IS_MAIN);
  }
}

//---------------------------------------------------------------------------
void Restful::setup(const json &configuration) {
  try {
    auto jsconfig = configuration;
  } catch (const std::exception &ex) {
    std::cout << "Failure during config: " << ex.what() << std::endl;
  }
}

//---------------------------------------------------------------------------
void Restful::run() {
  App::run();
  start();
  idle();
}

//---------------------------------------------------------------------------
auto main(int argc, char *argv[]) -> int {
  App::main(argc, argv, VERSION, new Restful(APP_NAME));
  return 0;
}