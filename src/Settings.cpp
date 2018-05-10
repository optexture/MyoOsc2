//
//  Settings.cpp
//  MyoOsc2
//

#include "Settings.h"
#include "optionparser.h"
#include <iostream>
#include <vector>

struct Arg : public option::Arg {
  static void printError(const char* msg1, const option::Option& opt, const char* msg2)
  {
    std::cerr << msg1 << std::string(opt.name, opt.namelen) << msg2;
  }

  static option::ArgStatus Unknown(const option::Option& option, bool msg)
  {
    if (msg) printError("Unknown option '", option, "'\n");
    return option::ARG_ILLEGAL;
  }
};

enum OptionIndex {
  UNKNOWN,
  HELP,
  LOG,
};
enum OptionType {DISABLE, ENABLE, OTHER};

const char usageText[] =
"USAGE: MyoOsc2 [options] <host> <port>\n"
"   MyoOsc2 sends OSC output over UDP from the input of one or more Thalmic Myo armbands.\n"
"   IP address defaults to 127.0.0.1/localhost\n"
"   Port defaults to 7777\n"
"   by tekt@optexture.com\n"
"   based on myo-osc by Samy Kamkar -- code@samy.pl\n";

const option::Descriptor usage[] =
{
  {UNKNOWN,     0,            "",   "",           Arg::Unknown,   usageText},
  {LOG,         ENABLE,       "l",  "log",        Arg::None,      "--log Enable OSC debug logging."},
  {HELP,        0,            "",   "help",       Arg::None,      "--help Print usage and exit."},
  {0, 0, 0, 0, 0, 0},
};

bool Settings::parseArgs(int argc, char **argv) {
  argc-=(argc>0); argv+=(argc>0); // skip program name argv[0] if present
  option::Stats  stats(usage, argc, argv);
  std::vector<option::Option> options(stats.options_max);
  std::vector<option::Option> buffer(stats.buffer_max);
  option::Parser parse(usage, argc, argv, options.data(), buffer.data());

  if (parse.error()) {
    return false;
  }

  if (options[HELP]) {
    option::printUsage(std::cerr, usage);
    return false;
  }

  port = 7777;
  hostname = "127.0.0.1";
  logging = true;

  for (const auto& opt : options) {
    switch (opt.index()) {
      case LOG:
        logging = opt.type() == ENABLE;
        break;
      case UNKNOWN:
        std::cerr << "Unknown option: " << std::string(opt.name, opt.namelen) << "\n\n";
        option::printUsage(std::cerr, usage);
        return false;
      default:
        break;
    }
  }
  if (parse.nonOptionsCount() == 2) {
    hostname = parse.nonOption(0);
    port = std::atoi(parse.nonOption(1));
  } else if (parse.nonOptionsCount() == 1) {
    port = std::atoi(parse.nonOption(0));
  } else if (parse.nonOptionsCount() != 0) {
    std::cerr << "strange number of non-option arguments: " << parse.nonOptionsCount() << "\n\n";
    option::printUsage(std::cerr, usage);
    return false;
  }
  return true;
}
