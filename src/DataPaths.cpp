//
//  DataPaths.cpp
//  MyoOsc2
//

#include "DataPaths.h"

static DeviceDataPaths unknownPaths("/__/");

DataPaths::DataPaths()
: _arms {
  DeviceDataPaths::byArm(myo::Arm::armLeft),
  DeviceDataPaths::byArm(myo::Arm::armRight)
} {}

const DeviceDataPaths& DataPaths::operator[](MyoId id) {
  while (id >= _devices.size()) {
    _devices.push_back(DeviceDataPaths::byDeviceId(id));
  }
  return _devices[id];
}

const DeviceDataPaths& DataPaths::operator[](myo::Arm arm) const {
  if (arm < 0 || arm >= numArms) {
    return unknownPaths;
  }
  return _arms[static_cast<std::size_t>(arm)];
}

DeviceDataPaths DeviceDataPaths::byDeviceId(MyoId id) {
  std::string base = "/myo/" + std::to_string(static_cast<int>(id)) + "/";
  return DeviceDataPaths(base);
}

DeviceDataPaths DeviceDataPaths::byArm(myo::Arm arm) {
  switch (arm) {
    case myo::Arm::armLeft:
      return DeviceDataPaths("/arm/left/");
    case myo::Arm::armRight:
      return DeviceDataPaths("/arm/right/");
    default:
      return unknownPaths;
  }
}

DeviceDataPaths::DeviceDataPaths(std::string base) {
  paired = base + "paired";
  connected = base + "connected";
  synced = base + "synced";
  arm = base + "arm";
  deviceDirection = base + "deviceDirection";
  armRotation = base + "armRotation";
  warmupState = base + "warmupState";
  locked = base + "locked";

  // poses
  for (auto i = 0; i < numPoses; i++) {
    poses[i] = base + "pose/" + myo::Pose(static_cast<myo::Pose::Type>(i)).toString();
  }

  // orientQuat
  orientQuat[0] = base + "orientQuat/x";
  orientQuat[1] = base + "orientQuat/y";
  orientQuat[2] = base + "orientQuat/z";
  orientQuat[3] = base + "orientQuat/w";

  // orientVec
  orientVec[0] = base + "orientVec/x";
  orientVec[1] = base + "orientVec/y";
  orientVec[2] = base + "orientVec/z";

  // accel
  accel[0] = base + "accel/x";
  accel[1] = base + "accel/y";
  accel[2] = base + "accel/z";

  // gyro
  gyro[0] = base + "gyro/x";
  gyro[1] = base + "gyro/y";
  gyro[2] = base + "gyro/z";

  rssi = base + "rssi";
  battery = base + "battery";

  // emg
  for (auto i = 0; i < emgLength; i++) {
    emg[i] = base + "emg/" + std::to_string(i);
  }

  warmupResult = base + "warmupResult";
}
