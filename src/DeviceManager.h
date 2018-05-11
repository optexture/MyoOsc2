//
//  DeviceManager.h
//  MyoOsc2
//

#pragma once

#include "Common.h"
#include "DataPaths.h"
#include <array>
#include <iostream>
#include <memory>
#include <vector>

class DeviceState {
public:
  DeviceState(MyoId i, MyoPtr d) : deviceId(i), paths(i), device(d) {}

  void clear() {
    device = nullptr;
    poses.fill(false);
  }

  const MyoId deviceId;
  const DeviceDataPaths paths;

  MyoPtr device;
  std::array<bool, numPoses> poses;
};

std::ostream& operator<<(std::ostream& os, const DeviceState& state);

using DeviceStateList = std::vector<DeviceState>;

class DeviceManager {
public:
  DeviceState& registerDevice(MyoPtr device);
  MyoId unregisterDevice(MyoPtr device);
  DeviceState& operator[](MyoPtr device) {
    return registerDevice(device);
  }
  DeviceStateList::iterator begin() { return _devices.begin(); }
  DeviceStateList::iterator end() { return _devices.end(); }
private:
  DeviceStateList _devices;
};
