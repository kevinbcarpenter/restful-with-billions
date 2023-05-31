#include "restful.h"
#include "http/host.h"
#include "http/listener.h"
#include <Logger.h>

#include <exception>
#include <memory>
#include <string>
#include <thread>

namespace {
const std::string VERSION{"1.0.0"};
const std::string APP_NAME{"restful"};

auto logInfo(const std::string &func) { return "Restufl: " + func; }

} // namespace

//---------------------------------------------------------------------------
Restful::Restful(std::string appName) : App(appName) {
  // nothing for now.
}

//---------------------------------------------------------------------------
void Restful::start() {
  http::host host;
  host.setup(App::getConfig().at("httpServer"));

  std::cout << "Starting server on " << host.ip << ":" << host.port
            << std::endl;

  std::thread(&http::listener::run, std::move(host)).detach();
}

//---------------------------------------------------------------------------
void Restful::idle() {
  const auto fname = ::logInfo(static_cast<const char *>(__func__));
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
int main(int argc, char *argv[]) {
  App::main(argc, argv, VERSION, new Restful(APP_NAME));
  return 0;
}