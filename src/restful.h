#pragma once
#include <App.h>
#include <future>

#include <httplib.h>
#include <json.hpp>

class Restful : public App {
private:
  void doIdle();

  void setup(const json &config) override;

  void start();

  void run() override;

public:
  Restful(std::string appName);
  ~Restful() = default;
};