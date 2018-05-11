#include "Logger.h"
#include <iomanip>

std::ostream& Logger::log() {
  if (!_settings.logging) {
    return _nullStream;
  }
  return std::cout;
}

std::ostream& Logger::verbose() {
  if (!_settings.verbose) {
    return _nullStream;
  }
  return std::cout;
}

std::ostream& Logger::verboseMessage(const std::string& path) {
  if (!_settings.verbose) {
    return _nullStream;
  }
  return std::cout << std::setw(20) << std::setfill(' ') << std::left << (path + ':');
}

void Logger::verboseMessage(const std::string& path, float val) {
  verboseMessage(path) << "  " << std::right << std::setprecision(2) << val << '\n';
}

void Logger::verboseMessage(const std::string& path, std::int8_t val) {
  verboseMessage(path) << "  " << std::setw(10) << std::right << val << '\n';
}

void Logger::verboseMessage(const std::string& path, std::uint8_t val) {
  verboseMessage(path) << "  " << std::setw(10) << std::right << val << '\n';
}

void Logger::verboseMessage(const std::string& path, bool val) {
  verboseMessage(path) << "  " << std::boolalpha << std::right << val << '\n';
}

void Logger::verboseMessage(const std::array<std::string, 3>& path,
                            const myo::Vector3<float>& val) {
  if (!_settings.verbose) {
    return;
  }
  verboseMessage(path[0], val.x());
  verboseMessage(path[1], val.y());
  verboseMessage(path[2], val.z());
}

void Logger::verboseMessage(const std::array<std::string, 4> &path,
                            const myo::Quaternion<float> &val) {
  if (!_settings.verbose) {
    return;
  }
  verboseMessage(path[0], val.x());
  verboseMessage(path[1], val.y());
  verboseMessage(path[2], val.z());
  verboseMessage(path[3], val.w());
}

void Logger::verboseMessage(const std::array<std::string, emgLength> &path,
                            const std::int8_t *vals) {
  if (!_settings.verbose) {
    return;
  }
  for (auto i = 0; i < emgLength; i++) {
    verboseMessage(path[i], vals[i]);
  }
}
