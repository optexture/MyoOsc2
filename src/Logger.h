#pragma once

#include <iomanip>
#include <iostream>
#include <NullOStream.h>
#include "Common.h"
#include "Settings.h"

class Logger {
public:
  explicit Logger(const Settings& settings) : _settings(settings) {}

  std::ostream& log();
  std::ostream& verbose();

  std::ostream& verboseMessage(const std::string& path);

  template<
    typename T,
    typename = std::enable_if_t<std::is_floating_point_v<T> > >
  void verboseMessage(const std::string& path, T val) {
    verboseMessage(path) << "  " << std::right << std::setprecision(2) << val << '\n';
  }

  template<
    typename T,
    typename = std::enable_if_t<std::is_integral_v<T> > >
  void verboseMessage(const std::string& path, T val) {
    verboseMessage(path) << "  " << std::setw(10) << std::right << val << '\n';
  }

  void verboseMessage(const std::string& path, bool val) {
    verboseMessage(path) << "  " << std::boolalpha << std::right << val << '\n';
  }
private:
  const Settings& _settings;
  NulOStream _nullStream;
};
