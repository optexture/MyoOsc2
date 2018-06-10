//
//  Common.hpp
//  MyoOsc2
//

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <myo.hpp>

using MyoPtr = myo::Myo*;

using MyoId = std::size_t;

const MyoId unknownMyoId = -1;

const std::size_t emgLength = 8;

const std::size_t numPoses = libmyo_num_poses;

const std::size_t numArms = 2;

using EmgArray = std::array<std::int8_t, emgLength>;
using PoseArray = std::array<bool, numPoses>;

std::ostream&
operator<<(std::ostream& os,
           const myo::FirmwareVersion& value);

std::ostream&
operator<<(std::ostream& os,
           const myo::Arm& value);

std::ostream&
operator<<(std::ostream& os,
           const myo::XDirection& value);

template<typename T>
std::ostream&
operator<<(std::ostream& os,
           const myo::Vector3<T>& value) {
  return os << "(" << value.x() << ", "
  << value.y() << ", " << value.z() << ")";
}

template<typename T>
std::ostream&
operator<<(std::ostream& os,
           const myo::Quaternion<T>& value) {
  return os << "(" << value.x() << ", "
  << value.y() << ", " << value.z()
  << value.w() << ")";
}

std::ostream&
operator<<(std::ostream& os,
           const EmgArray& value);
