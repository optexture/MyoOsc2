//
//  main.cpp
//  MyoOsc2
//

#include <iostream>
#include <myo.hpp>
#include <stdexcept>
#include "Common.h"
#include "DeviceManager.h"
#include "DataSender.h"
#include "Settings.h"

int main(int argc, char * argv[]) {
  Settings settings;

  try {
    if (!settings.parseArgs(argc, argv)) {
      return 1;
    }

    std::cout << "Sending Myo OSC to " << settings.hostname << ":" << settings.port << "\n";

    DeviceManager devices;
    DataSender sender(devices, settings);

    myo::Hub hub("com.optexture.myoosc2");
    hub.addListener(&sender);

    while (true) {
      hub.run(1000 / 20);
      for (auto& state : devices) {
        if (state.device) {
          state.device->requestRssi();
          state.device->requestBatteryLevel();
        }
      }
    }

  } catch (const std::exception& e) {
    // If a standard exception occurred, we print out its message and exit.
    std::cerr << "Error: " << e.what() << std::endl;
    std::cerr << "Press enter to continue.";
    std::cin.ignore();
    return 1;
  }

  return 0;
}
