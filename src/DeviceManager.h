//
//  DeviceManager.h
//  MyoOsc2
//

#pragma once

#include "Common.h"
#include "DataPaths.h"
#include <array>
#include <memory>
#include <vector>

class DeviceState {
public:
  explicit DeviceState(MyoId i) : deviceId(i), paths(i) {}

  void clear() {
    device = nullptr;
    poses.fill(false);
  }

  const MyoId deviceId;
  const DeviceDataPaths paths;

  MyoPtr device;
  std::array<bool, numPoses> poses;
};

using DeviceStateList = std::vector<DeviceState>;

class DeviceManager {
public:
  DeviceState& registerDevice(MyoPtr device);
  void unregisterDevice(MyoPtr device);
  DeviceState& operator[](MyoPtr device) {
    return registerDevice(device);
  }
  DeviceStateList::iterator begin() { return _devices.begin(); }
  DeviceStateList::iterator end() { return _devices.end(); }
private:
  DeviceStateList _devices;
};
