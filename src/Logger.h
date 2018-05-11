#pragma once

#include <array>
#include <iostream>
#include <myo.hpp>
#include "NullOStream.h"
#include "Common.h"
#include "Settings.h"

class Logger {
public:
  explicit Logger(const Settings& settings) : _settings(settings) {}

  std::ostream& log();
  std::ostream& verbose();

  std::ostream& verboseMessage(const std::string& path);

  void verboseMessage(const std::string& path, float val);

  void verboseMessage(const std::string& path, std::int8_t val);

  void verboseMessage(const std::string& path, std::uint8_t val);

  void verboseMessage(const std::string& path, bool val);

  void verboseMessage(const std::array<std::string, 3>& path,
                      const myo::Vector3<float>& val);

  void verboseMessage(const std::array<std::string, 4>& path,
                      const myo::Quaternion<float>& val);

  void verboseMessage(const std::array<std::string, emgLength>& path,
                      const std::int8_t* vals);
private:
  const Settings& _settings;
  NulOStream _nullStream;
};
