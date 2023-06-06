#pragma once
#include <App.h>

#include <json.hpp>

class Restful : public App {
private:
  void setup(const json &configuration) override;
  void start();
  void idle();

public:
  Restful(std::string appName);
  ~Restful() final = default;
  void run() override;
};