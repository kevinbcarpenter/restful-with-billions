#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <execinfo.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <set>
#include <signal.h>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std;

#include "App.h"
#include "Logger.h"
#include "StringUtils.h"

namespace {
void saveStackTrace(int fd) {
  // Get our backtrace
  void *array[25];
  int nSize = backtrace(array, 25);

  // Log backtrace
  auto timeData = time(nullptr);
  char timeCStr[26];
  ctime_r(&timeData, timeCStr);
  assert(write(fd, timeCStr, 25) == 25);
  assert(write(fd, "== SIGSEGV ==\n", 15) == 15);

  backtrace_symbols_fd(array, nSize, fd);
  assert(write(fd, "\n", 1) == 1);
}

struct BacktraceWarmup {
  BacktraceWarmup();
};

BacktraceWarmup::BacktraceWarmup() {
  auto devNull = open("/dev/null", O_WRONLY);
  assert(devNull != -1);
  saveStackTrace(devNull);
  assert(close(devNull) == 0);
}

static BacktraceWarmup warmup;
} // namespace

//---------------------------------------------------------------------------
void App::saveStackTrace() {
  // Open our crash log
  auto fd = open("crash.log", O_APPEND | O_CREAT | O_WRONLY, 0644);
  assert(fd != -1);
  ::saveStackTrace(fd);
  assert(close(fd) == 0);
}

//---------------------------------------------------------------------------
void App::handleSIGSEGV(int sig) {
  // Save out and die
  saveStackTrace();
  // Added to cause core dump.  Use pthread_exit to not core dump but still log.
  signal(sig, SIG_DFL);
  kill(getpid(), sig);
  // pthread_exit( 0 );
}

//---------------------------------------------------------------------------
void App::handleSIGINT(int sig __attribute__((unused))) {
  // This isn't going to be pretty
  cout << __func__ << endl;
  exit(0);
}

//---------------------------------------------------------------------------
void App::dumpError(string error) {
  saveStackTrace();
  Logger::appError(error);
}

//---------------------------------------------------------------------------
json App::readConfiguration() {
  json jsonConfig;
  try {
    ifstream configFile(string(appName + ".json").c_str());
    if (configFile.ios::eof()) {
      throw runtime_error("Config file is empty");
    }

    // Make sure the config is open
    if (!configFile.is_open()) {
      throw runtime_error("Can't open config");
    }

    configFile >> jsonConfig;

  } catch (exception &e) {
    throw string(e.what());

  } catch (...) {
    throw "Can't open config";
  }
  return jsonConfig;
}

// ---------------------------------------------------------------------------
void App::configUpdate(const json &config) {
  // DUMP_PACKETS_CFG
  bool dumpReset = config["dumps"]["on"].get<bool>();
  if (dumpReset != Logger::dump()) {
    ostringstream logEntry;
    Logger::setDump(dumpReset);
    logEntry << "Dump packets reset: " +
                    (Logger::dump() ? string("on") : string("off"));
    logger->save(App::logInfo(__func__), logEntry.str(), IS_THREAD);
  }
}

// ---------------------------------------------------------------------------
void App::handleConfigUpdate() {
  json tempConfiguration = readConfiguration();
  configUpdate(tempConfiguration);
}

//---------------------------------------------------------------------------
void App::configThread() {
  while (true) {
    try {
      this_thread::sleep_for(chrono::seconds(10));
      handleConfigUpdate();
    } catch (...) {
    }
  }
}

// ---------------------------------------------------------------------------
void App::startConfigThread() {
  thread t = thread([this] { configThread(); });
  t.detach();
}

//---------------------------------------------------------------------------
#ifndef NO_ZMQ_SUPPORT
App::App(string appName) : context(1), appName(appName) { ; }
#else
App::App(string appName) : appName(appName) { ; }
#endif
//---------------------------------------------------------------------------
App::~App() {}
//---------------------------------------------------------------------------
bool App::versionCheck(int argc, char *argv[], string version) {
  if (argc == 2) {
    string versionCheck = argv[1];
    transform(versionCheck.begin(), versionCheck.end(), versionCheck.begin(),
              ::toupper);

    if (versionCheck.compare("-V") == 0) {
      cout << appName << " version: " << version << endl;
      return true;
    }
  }

  return false;
}

//---------------------------------------------------------------------------
void App::configureLogger() {
  // Setup logger
  logger = Logger::getLogger();

  // Log location
  auto logLocation = configuration["logs"]["location"];
  if (logLocation.is_null()) {
    throw "Missing log location";
  }
  logger->setLogLocation(this->appName, logLocation);

  // Setup dumps if used
  auto dumps = configuration["dumps"];
  if (!dumps.is_null()) {
    auto dumpLocation = dumps["location"];
    bool dumpsOn = dumps["on"];
    if (dumpLocation.is_null()) {
      throw "Missing dump location";
    }

    // Set and turn on dumps
    logger->setDumpLocation(dumpLocation);
    Logger::setDump(dumpsOn);
  }
}

#ifdef STATE_SUPPORT
//---------------------------------------------------------------------------
void App::configureStateMachine() {
  AppState::createInstance(getConfig(), getContext(), appName);
  appState = AppState::getInstance();
}

//--------------------------------------------------------------------------
void App::configureStateSubscription() {
  StateSub::createInstance(getContext(), getConfig(), stateLevel);
  stateSub = StateSub::getInstance();
}
#endif

//--------------------------------------------------------------------------
void App::run() {
  // Install signal handler
  if (installSegVHandler()) {
    signal(SIGSEGV, &App::handleSIGSEGV);
  }

  if (installSegIntHandler()) {
    signal(SIGINT, &App::handleSIGINT);
  }

  // Seed our random number generator
  srand((unsigned)time(0));

  // Setup configuration
  configuration = readConfiguration();
  configureLogger();

#ifdef STATE_SUPPORT
  configureStateSubscription(); // State Subscription MUST be set before State
                                // Machine
  configureStateMachine();
#endif

  // Call the subclass with the configuration
  setup(configuration);

  // Start the config background thread if needed
  if (shouldStartConfigThread()) {
    startConfigThread();
  }
}

//---------------------------------------------------------------------------
const json &App::getConfig() const { return configuration; }

#ifndef NO_ZMQ_SUPPORT
//---------------------------------------------------------------------------
zmq::context_t &App::getContext() { return context; }
#endif

//---------------------------------------------------------------------------
void App::main(int argc, char *argv[], const string &version, App *app) {
  // Create ourself, and run
  try {
    // See if this is just a version check
    if (app->versionCheck(argc, argv, version)) {
      goto complete;
    }

    // Run forever
    app->run();

  } catch (string s) {
    app->dumpError(s);
  } catch (const char *s) {
    app->dumpError(s);
  } catch (exception &e) {
    app->dumpError(e.what());
  } catch (...) {
    app->dumpError("Unknown");
  }

// Cleanup
complete:
#ifdef STATE_SUPPORT
  if (app->appState) {
    delete app->appState;
  }
  if (app->stateSub) {
    delete app->stateSub;
  }
#endif

  if (app) {
    delete app;
  }
}
