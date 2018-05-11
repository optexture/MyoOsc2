//
//  DeviceManager.cpp
//  MyoOsc2
//

#include "DeviceManager.h"
#include <iomanip>

std::ostream& operator<<(std::ostream& os, MyoPtr device) {
  return os << std::hex << std::showbase << std::internal << std::setw(8) << std::setfill('0') << std::addressof(device);
}

std::ostream& operator<<(std::ostream& os, const DeviceState& state) {
  return os << "device " << state.deviceId << " [" << state.device << "]";
}

DeviceState& DeviceManager::registerDevice(MyoPtr device) {
  for (DeviceState& state : _devices) {
    if (state.device == device) {
      return state;
    }
  }
  for (MyoId id = 0; id < _devices.size(); id++) {
    DeviceState& state = _devices[id];
    if (!state.device) {
      state.device = device;
      return state;
    }
  }
  _devices.emplace_back(_devices.size(), device);
  return _devices.back();
}

MyoId DeviceManager::unregisterDevice(MyoPtr device) {
  for (DeviceState& state : _devices) {
    if (state.device == device) {
      auto id = state.deviceId;
      state.clear();
      return id;
    }
  }
  return unknownMyoId;
}
