//
//  DataSender.cpp
//  MyoOsc2
//

#include "DataSender.h"
#include <iomanip>

static myo::Vector3<float> quaternionToVector(const myo::Quaternion<float>& quat) {
  // Calculate Euler angles (roll, pitch, and yaw) from the unit quaternion.
  float yaw = atan2(2.0f * (quat.w() * quat.z() + quat.x() * quat.y()),
                    1.0f - 2.0f * (quat.y() * quat.y() + quat.z() * quat.z()));
  float pitch = asin(2.0f * (quat.w() * quat.y() - quat.z() * quat.x()));
  float roll = atan2(2.0f * (quat.w() * quat.x() + quat.y() * quat.z()),
                     1.0f - 2.0f * (quat.x() * quat.x() + quat.y() * quat.y()));
  return myo::Vector3<float>(yaw, pitch, roll);
}

DataSender::DataSender(DeviceManager& devices,
                       const Settings& settings)
: _devices(devices)
, _settings(settings)
, _socket(std::make_unique<UdpTransmitSocket>(IpEndpointName(settings.hostname.c_str(), settings.port))) {}

void DataSender::logPath(const std::string& path) {
  std::cout << std::setw(20) << std::setfill(' ') << std::left << (path + ":");
}

void DataSender::logVal(float val) {
  std::cout << "  " << std::setw(10) << std::right << std::setprecision(2) << val;
}

void DataSender::logVal(int8_t val) {
  std::cout << "  " << std::setw(10) << std::right << (int)val;
}

void DataSender::logVal(uint8_t val) {
  std::cout << "  " << std::setw(10) << std::right << (int)val;
}

void DataSender::logVal(bool val) {
  std::cout << "  " << std::boolalpha << std::right << val;
}

void DataSender::logVector(const myo::Vector3<float>& vec) {
  logVal(vec.x());
  logVal(vec.y());
  logVal(vec.z());
}

void DataSender::logQuaterion(const myo::Quaternion<float>& quat) {
  logVal(quat.x());
  logVal(quat.y());
  logVal(quat.z());
  logVal(quat.w());
}

osc::OutboundPacketStream DataSender::beginMessage(const std::string& path) {
  osc::OutboundPacketStream p(_buffer, OUTPUT_BUFFER_SIZE);
  p << osc::BeginMessage(path.c_str());
  return p;
}

void DataSender::send(const osc::OutboundPacketStream& p) {
  _socket->Send(p.Data(), p.Size());
}

void DataSender::sendMessage(const std::array<std::string, emgLength>& path,
                             const int8_t* vals) {
  for (auto i = 0; i < emgLength; i++) {
    sendMessage(path[i], vals[i]);
  }
}

void DataSender::sendMessage(const std::string& path, const char* val) {
  send(beginMessage(path)
       << val << osc::EndMessage);
  if (_settings.logging) {
    logPath(path);
    std::cout << "  " << std::right << val;
    std::cout << std::endl;
  }
}

void DataSender::sendMessage(const std::array<std::string, 3>& path,
                             myo::Vector3<float> vec) {
  sendMessage(path[0], vec.x());
  sendMessage(path[1], vec.y());
  sendMessage(path[2], vec.z());
}

void DataSender::sendMessage(const std::array<std::string, 4>& path,
                             myo::Quaternion<float> quat) {
  sendMessage(path[0], quat.x());
  sendMessage(path[1], quat.y());
  sendMessage(path[2], quat.z());
  sendMessage(path[3], quat.w());
}

void DataSender::onPair(MyoPtr device, uint64_t timestamp,
                        myo::FirmwareVersion firmwareVersion) {
  sendMessage(devicePaths(device).paired, true);
}

void DataSender::onUnpair(MyoPtr device, uint64_t timestamp) {
  sendMessage(devicePaths(device).paired, false);
}

void DataSender::onConnect(MyoPtr device, uint64_t timestamp, myo::FirmwareVersion firmwareVersion) {
  device->setStreamEmg(myo::Myo::streamEmgEnabled);
  // unlock..?
  sendMessage(devicePaths(device).connected, true);
}

void DataSender::onDisconnect(MyoPtr device, uint64_t timestamp) {
  sendMessage(devicePaths(device).connected, false);
  _devices.unregisterDevice(device);
}

void DataSender::onArmSync(MyoPtr device, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation, myo::WarmupState warmupState) {
  const auto& paths = devicePaths(device);
  sendMessage(paths.synced, true);
  sendMessage(paths.arm, static_cast<int8_t>(arm));
  sendMessage(paths.deviceDirection, static_cast<int8_t>(xDirection));
  sendMessage(paths.armRotation, static_cast<int8_t>(rotation));
  sendMessage(paths.warmupState, static_cast<int8_t>(warmupState));
}

void DataSender::onArmUnsync(MyoPtr device, uint64_t timestamp) {
  const auto& paths = devicePaths(device);
  sendMessage(paths.synced, false);
}

void DataSender::onUnlock(MyoPtr device, uint64_t timestamp) {
  sendMessage(devicePaths(device).locked, false);
}

void DataSender::onLock(MyoPtr device, uint64_t timestamp) {
  sendMessage(devicePaths(device).locked, true);
}

void DataSender::onPose(MyoPtr device, uint64_t timestamp, myo::Pose pose) {
  auto& state = _devices[device];
  auto i = static_cast<std::size_t>(pose.type());
  sendMessage(state.paths.poses[i], true);
  state.poses[i] = true;
}

void DataSender::flushPoseStates() {
  for (auto& state : _devices) {
    for (auto i = 0; i < numPoses; i++) {
      if (state.poses[i]) {
        sendMessage(state.paths.poses[i], true);
        state.poses[i] = false;
      }
    }
  }
}

void DataSender::onOrientationData(MyoPtr device, uint64_t timestamp, const myo::Quaternion<float> &rotation) {
  const auto& paths = devicePaths(device);
  sendMessage(paths.orientQuat, rotation);
  sendMessage(paths.orientVec, quaternionToVector(rotation));
}

// units of g
void DataSender::onAccelerometerData(MyoPtr device, uint64_t timestamp,
                                     const myo::Vector3<float>& accel)
{
  sendMessage(devicePaths(device).accel, accel);
}

void DataSender::onGyroscopeData(MyoPtr device, uint64_t timestamp,
                                 const myo::Vector3<float> &gyro) {
  sendMessage(devicePaths(device).gyro, gyro);
}

void DataSender::onRssi(MyoPtr device, uint64_t timestamp, int8_t rssi) {
  sendMessage(devicePaths(device).rssi, rssi);
}

void DataSender::onBatteryLevelReceived(MyoPtr device, uint64_t timestamp, uint8_t level) {
  sendMessage(devicePaths(device).battery, level);
}

void DataSender::onEmgData(MyoPtr device, uint64_t timestamp, const int8_t *emg) {
  sendMessage(devicePaths(device).emg, emg);
}

void DataSender::onWarmupCompleted(MyoPtr device, uint64_t timestamp, myo::WarmupResult warmupResult) {
  sendMessage(devicePaths(device).warmupResult, static_cast<int8_t>(warmupResult));
}
