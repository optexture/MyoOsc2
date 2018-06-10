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
  DeviceState(MyoId i, MyoPtr d, DataPaths& dataPaths)
  : deviceId(i)
  , paths(dataPaths[i])
  , device(d)
  , arm(myo::Arm::armUnknown) {}

  void clear() {
    device = nullptr;
    poses.fill(false);
    arm = myo::Arm::armUnknown;
  }

  bool armValid() const {
    return arm == myo::Arm::armLeft || arm == myo::Arm::armRight;
  }

  const MyoId deviceId;
  const DeviceDataPaths paths;

  MyoPtr device;
  myo::Arm arm;
  std::array<bool, numPoses> poses;
};

std::ostream& operator<<(std::ostream& os, const DeviceState& state);

using DeviceStateList = std::vector<DeviceState>;

class DeviceManager {
public:
  DeviceManager(DataPaths& dataPaths) : _dataPaths(dataPaths) {}

  DeviceState& registerDevice(MyoPtr device);
  MyoId unregisterDevice(MyoPtr device);
  DeviceState& operator[](MyoPtr device) {
    return registerDevice(device);
  }
  DeviceStateList::iterator begin() { return _devices.begin(); }
  DeviceStateList::iterator end() { return _devices.end(); }
private:
  DeviceStateList _devices;
  DataPaths& _dataPaths;
};
