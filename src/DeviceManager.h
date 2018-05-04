//
//  DeviceManager.h
//  MyoOsc2
//

#pragma once

#include "Common.h"
#include <vector>

class DeviceManager {
public:
  MyoId registerDevice(MyoPtr device);
  void unregisterDevice(MyoPtr device);
private:
  MyoId getDeviceId(MyoPtr device) const;

  std::vector<MyoPtr> _devices;
};
