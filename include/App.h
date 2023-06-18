#ifndef App_class
#define App_class

#include <string>
#include <vector>

#ifndef NO_ZMQ_SUPPORT
#include "zmq.hpp"
#endif

#include "json.hpp"
using json = nlohmann::json;

#ifdef STATE_SUPPORT
#include "AppState.h"
#include "StateSub.h"
#else
#include "AppResources.h"
#endif

/***********************************************************************
** Forward Declarations
***********************************************************************/
class Logger;

/***********************************************************************
** Class Declaration
***********************************************************************/
class App {
private:
#ifndef NO_ZMQ_SUPPORT
  zmq::context_t context;
#endif
  std::string appName;
  std::vector<std::string> loadBalancers;

  // Configuration
  json configuration;
  json readConfiguration();
  void configThread();
  void startConfigThread();

  // Stack debugging
  static void saveStackTrace();
  static void handleSIGSEGV(int sig);
  static void handleSIGINT(int sig);

  void handleConfigUpdate();

  // Logging
  void configureLogger();

  static std::string logInfo(const std::string &function) {
    return "App:" + function;
  }

#ifdef STATE_SUPPORT
  void configureStateMachine();
  void configureStateSubscription();
#endif

protected:
  Logger *logger{nullptr};

#ifdef STATE_SUPPORT
  AppState *appState{nullptr};
  StateSub *stateSub{nullptr};
  StateLevel stateLevel{StateLevel::MCP};
#endif

  virtual void setup(const json &configuration) = 0;

  // Always start by default
  virtual bool shouldStartConfigThread() { return true; }

  virtual bool installSegVHandler() { return true; }

  virtual bool installSegIntHandler() { return false; }

  virtual void configUpdate(const json &config);

public:
  App(std::string appName);
  virtual ~App();
  virtual void run();

  App(const App &) = delete;
  App &operator=(const App &) = delete;
  App(App &&) = delete;
  App &operator=(App &&) = delete;

  static void main(int argc, char *argv[], const std::string &version,
                   App *app);

  bool versionCheck(int argc, char *argv[], std::string version);
  void dumpError(std::string error);
  const json &getConfig() const;

#ifndef NO_ZMQ_SUPPORT
  zmq::context_t &getContext();
#endif
};

#endif
