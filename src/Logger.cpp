#include "Logger.h"

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
