//
//  Settings.h
//  MyoOsc2
//
//

#pragma once

#include <string>

class Settings {
public:
  Settings()
  : hostname("localhost")
  , port(7777)
  , logging(false) {}

  bool parseArgs(int argc, char **argv);

  std::string hostname;
  int port;
  bool logging;
};
