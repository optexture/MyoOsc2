//
//  DataPaths.h
//  MyoOsc2
//

#pragma once

#include <array>
#include <string>
#include <vector>
#include "Common.h"

class DeviceDataPaths {
public:
  explicit DeviceDataPaths(MyoId id);

  std::string paired;
  std::string connected;
  std::string synced;
  std::string arm;
  std::string deviceDirection;
  std::string armRotation;
  std::string warmupState;
  std::string locked;
  std::array<std::string, numPoses> poses;
  std::array<std::string, 4> orientQuat;
  std::array<std::string, 3> orientVec;
  std::array<std::string, 3> accel;
  std::array<std::string, 3> gyro;
  std::string rssi;
  std::string battery;
  std::array<std::string, emgLength> emg;
  std::string warmupResult;
};

class DataPaths {
public:
  const DeviceDataPaths& operator[](MyoId id);
private:
  std::vector<DeviceDataPaths> _devices;
};
