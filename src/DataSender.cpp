//
//  DataSender.cpp
//  MyoOsc2
//

#include "DataSender.h"
#include <iomanip>

static myo::Vector3<float>
quaternionToVector(const myo::Quaternion<float>& quat) {
  // Calculate Euler angles (roll, pitch, and yaw) from the unit quaternion.
  float yaw = atan2(2.0f * (quat.w() * quat.z() + quat.x() * quat.y()),
                    1.0f - 2.0f * (quat.y() * quat.y() + quat.z() * quat.z()));
  float pitch = asin(2.0f * (quat.w() * quat.y() - quat.z() * quat.x()));
  float roll = atan2(2.0f * (quat.w() * quat.x() + quat.y() * quat.z()),
                     1.0f - 2.0f * (quat.x() * quat.x() + quat.y() * quat.y()));
  return myo::Vector3<float>(yaw, pitch, roll);
}

DataSender::DataSender(DeviceManager& devices,
                       const Settings& settings,
                       const DataPaths& dataPaths,
                       Logger& logger)
: _devices(devices)
, _settings(settings)
, _dataPaths(dataPaths)
, _logger(logger)
, _socket(std::make_unique<UdpTransmitSocket>(IpEndpointName(settings.hostname.c_str(), settings.port))) {}

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

void DataSender::sendMessage(const std::array<std::string, 3>& path,
                             myo::Vector3<float> vec) {
  send(path[0], vec.x());
  send(path[1], vec.y());
  send(path[2], vec.z());
  _logger.verboseMessage(path, vec);
}

void DataSender::sendMessage(const std::array<std::string, 4>& path,
                             myo::Quaternion<float> quat) {
  send(path[0], quat.x());
  send(path[1], quat.y());
  send(path[2], quat.z());
  send(path[3], quat.w());
  _logger.verboseMessage(path, quat);
}

void DataSender::onPair(MyoPtr device, uint64_t timestamp,
                        myo::FirmwareVersion firmwareVersion) {
  const auto& state = _devices[device];
  _logger.log() << "Paired " << state << std::endl;
  sendMessage(state.paths.paired, true);
}

void DataSender::onUnpair(MyoPtr device, uint64_t timestamp) {
  const auto& state = _devices[device];
  _logger.log() << "Unpaired " << state << std::endl;
  sendMessage(state.paths.paired, false);
}

void DataSender::onConnect(MyoPtr device, uint64_t timestamp, myo::FirmwareVersion firmwareVersion) {
  const auto& state = _devices[device];
  _logger.log() << "Connect " << state << std::endl;
  device->setStreamEmg(myo::Myo::streamEmgEnabled);
  // unlock..?
  sendMessage(state.paths.connected, true);
}

void DataSender::onDisconnect(MyoPtr device, uint64_t timestamp) {
  const auto& state = _devices[device];
  _logger.log() << "Disconnect " << state << std::endl;
  sendMessage(state.paths.connected, false);
  _devices.unregisterDevice(device);
}

void DataSender::onArmSync(MyoPtr device, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation, myo::WarmupState warmupState) {
  const auto& state = _devices[device];
  _logger.log() << "Arm sync " << state << std::endl;
  const auto& paths = state.paths;
  sendMessage(paths.synced, true);
  sendEnumMessage(paths.arm, arm);
  sendEnumMessage(paths.deviceDirection, xDirection);
  sendMessage(paths.armRotation, rotation);
  sendEnumMessage(paths.warmupState, warmupState);
}

void DataSender::onArmUnsync(MyoPtr device, uint64_t timestamp) {
  const auto& state = _devices[device];
  _logger.log() << "Arm unsync " << state << std::endl;
  sendMessage(state.paths.synced, false);
}

void DataSender::onUnlock(MyoPtr device, uint64_t timestamp) {
  const auto& state = _devices[device];
  _logger.log() << "Unlock " << state << std::endl;
  sendMessage(state.paths.locked, false);
}

void DataSender::onLock(MyoPtr device, uint64_t timestamp) {
  const auto& state = _devices[device];
  _logger.log() << "Lock " << state << std::endl;
  sendMessage(state.paths.locked, true);
}

void DataSender::onPose(MyoPtr device, uint64_t timestamp, myo::Pose pose) {
  if (pose.type() == myo::Pose::Type::unknown) {
    return;
  }
  auto& state = _devices[device];
  auto i = static_cast<std::size_t>(pose.type());
  sendMessage(state.paths.poses[i], true);
  state.poses[i] = true;
}

void DataSender::flushPoseStates() {
  for (auto& state : _devices) {
    for (auto i = 0; i < numPoses; i++) {
      if (state.poses[i]) {
        sendMessage(state.paths.poses[i], false);
        state.poses[i] = false;
      }
    }
  }
}

void DataSender::onOrientationData(MyoPtr device, uint64_t timestamp, const myo::Quaternion<float> &rotation) {
  auto vec = quaternionToVector(rotation);
  const auto& state = _devices[device];
  sendMessage(state.paths.orientQuat, rotation);
  sendMessage(state.paths.orientVec, vec);
  if (state.armValid()) {
    const auto& armPaths = _dataPaths[state.arm];
    sendMessage(armPaths.orientQuat, rotation);
    sendMessage(armPaths.orientVec, vec);
  }
}

// units of g
void DataSender::onAccelerometerData(MyoPtr device, uint64_t timestamp,
                                     const myo::Vector3<float>& accel)
{
  const auto& state = _devices[device];
  sendMessage(state.paths.accel, accel);
  if (state.armValid()) {
    const auto& armPaths = _dataPaths[state.arm];
    sendMessage(armPaths.accel, accel);
  }
}

void DataSender::onGyroscopeData(MyoPtr device, uint64_t timestamp,
                                 const myo::Vector3<float> &gyro) {
  const auto& state = _devices[device];
  sendMessage(state.paths.gyro, gyro);
  if (state.armValid()) {
    const auto& armPaths = _dataPaths[state.arm];
    sendMessage(armPaths.gyro, gyro);
  }
}

void DataSender::onRssi(MyoPtr device, uint64_t timestamp, int8_t rssi) {
  const auto& state = _devices[device];
  sendMessage(state.paths.rssi, rssi);
  if (state.armValid()) {
    const auto& armPaths = _dataPaths[state.arm];
    sendMessage(armPaths.rssi, rssi);
  }
}

void DataSender::onBatteryLevelReceived(MyoPtr device, uint64_t timestamp, uint8_t level) {
  sendMessage(devicePaths(device).battery, level);
}

void DataSender::onEmgData(MyoPtr device, uint64_t timestamp, const int8_t *emg) {
  const auto& state = _devices[device];
  sendMessage(state.paths.emg, emg);
  if (state.armValid()) {
    const auto& armPaths = _dataPaths[state.arm];
    sendMessage(armPaths.emg, emg);
  }
}

void DataSender::onWarmupCompleted(MyoPtr device, uint64_t timestamp, myo::WarmupResult warmupResult) {
  sendEnumMessage(devicePaths(device).warmupResult, warmupResult);
}
