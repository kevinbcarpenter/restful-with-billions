#pragma once
#include <App.h>

#include <json.hpp>

class Restful : public App {
private:
  void setup(const json &configuration) override;
  void start();
  static void idle();

public:
  Restful(std::string appName);
  ~Restful() final = default;

  Restful(const Restful &) = delete;
  Restful &operator=(const Restful &) = delete;
  Restful(Restful &&) = delete;
  Restful &operator=(Restful &&) = delete;

  void run() override;
};