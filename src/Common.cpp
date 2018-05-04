//
//  Common.cpp
//  MyoOsc2
//

#include "Common.h"

std::ostream&
operator<<(std::ostream& os,
           const myo::FirmwareVersion& value) {
  return os << value.firmwareVersionMajor << "."
  << value.firmwareVersionMinor << "."
  << value.firmwareVersionPatch << "."
  << value.firmwareVersionHardwareRev;
}

std::ostream&
operator<<(std::ostream& os,
           const myo::Arm& value) {
  switch (value) {
    case myo::armLeft:
      return os << "left";
    case myo::armRight:
      return os << "right";
    case myo::armUnknown:
    default:
      return os << "unknown";
  }
}

std::ostream&
operator<<(std::ostream& os,
           const myo::XDirection& value) {
  switch (value) {
    case myo::xDirectionTowardElbow:
      return os << "elbow";
    case myo::xDirectionTowardWrist:
      return os << "wrist";
    case myo::xDirectionUnknown:
    default:
      return os << "unknown";
  }
}

std::ostream&
operator<<(std::ostream& os,
           const EmgArray& value) {
  os << "(";
  for (std::size_t i = 0; i < emgLength; i++) {
    if (i > 0) {
      os << ", ";
    }
    os <<  static_cast<int>(value[i]);
  }
  return os << ")";
}
