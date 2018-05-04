//
//  DeviceManager.cpp
//  MyoOsc2
//

#include "DeviceManager.h"

MyoId DeviceManager::registerDevice(MyoPtr device) {
  auto id = getDeviceId(device);
  if (id != unknownMyoId) {
    return id;
  }
  for (id = 0; id < _devices.size(); id++) {
    if (_devices[id] == nullptr) {
      _devices[id] = device;
      return id;
    }
  }
  _devices.push_back(device);
  return _devices.size() - 1;
}

void DeviceManager::unregisterDevice(MyoPtr device) {
  auto id = getDeviceId(device);
  if (id != unknownMyoId) {
    _devices[id] = nullptr;
  }
}

MyoId DeviceManager::getDeviceId(MyoPtr device) const {
  for (MyoId id = 0; id < _devices.size(); id++) {
    if (_devices[id] == device) {
      return id;
    }
  }
  return unknownMyoId;
}
