//
//  DataSender.h
//  MyoOsc2
//

#pragma once

#include <memory>
#include <type_traits>
#include "ip/UdpSocket.h"
#include "osc/OscOutboundPacketStream.h"

#include "Common.h"
#include "DataPaths.h"
#include "DeviceManager.h"
#include "Logger.h"
#include "Settings.h"

#define OUTPUT_BUFFER_SIZE 1024

class DataSender : public myo::DeviceListener {
public:
  DataSender(DeviceManager& devices,
             const Settings& settings,
             Logger& logger);

  virtual ~DataSender() {}

  void flushPoseStates();

  /// Called when a Myo has been paired.
  /// @param device The Myo for this event.
  /// @param timestamp The timestamp of when the event is received by the SDK. Timestamps are 64 bit unsigned
  /// integers that correspond to a number of microseconds since some (unspecified) period in time. Timestamps
  /// are monotonically non-decreasing.
  /// @param firmwareVersion The firmware version of \a myo.
  void onPair(MyoPtr device, uint64_t timestamp, myo::FirmwareVersion firmwareVersion) override;

  /// Called when a Myo has been unpaired.
  /// @param device The Myo for this event.
  /// @param timestamp The timestamp of when the event is received by the SDK. Timestamps are 64 bit unsigned
  /// integers that correspond to a number of microseconds since some (unspecified) period in time. Timestamps
  /// are monotonically non-decreasing.
  void onUnpair(MyoPtr device, uint64_t timestamp) override;

  /// Called when a paired Myo has been connected.
  /// @param device The Myo for this event.
  /// @param timestamp The timestamp of when the event is received by the SDK. Timestamps are 64 bit unsigned
  /// integers that correspond to a number of microseconds since some (unspecified) period in time. Timestamps
  /// are monotonically non-decreasing.
  /// @param firmwareVersion The firmware version of \a myo.
  void onConnect(MyoPtr device, uint64_t timestamp, myo::FirmwareVersion firmwareVersion) override;

  /// Called when a paired Myo has been disconnected.
  /// @param device The Myo for this event.
  /// @param timestamp The timestamp of when the event is received by the SDK. Timestamps are 64 bit unsigned
  /// integers that correspond to a number of microseconds since some (unspecified) period in time. Timestamps
  /// are monotonically non-decreasing.
  void onDisconnect(MyoPtr device, uint64_t timestamp) override;

  /// Called when a paired Myo recognizes that it is on an arm.
  /// @param device The Myo for this event.
  /// @param timestamp The timestamp of when the event is received by the SDK. Timestamps are 64 bit unsigned
  /// integers that correspond to a number of microseconds since some (unspecified) period in time. Timestamps
  /// are monotonically non-decreasing.
  /// @param arm The identified Arm of \a myo.
  /// @param xDirection The identified XDirection of \a myo.
  /// @param rotation The estimated rotation of Myo on the user's arm after a sync.
  /// @param warmupState The warmup state of \a myo. If \a warmupState is equal to WarmupState::warmupStateCold,
  /// onWarmupCompleted() will be called when the warmup period has completed.
  void onArmSync(MyoPtr device, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation,
                         myo::WarmupState warmupState) override;

  /// Called when a paired Myo is moved or removed from the arm.
  /// @param device The Myo for this event.
  /// @param timestamp The timestamp of when the event is received by the SDK. Timestamps are 64 bit unsigned
  /// integers that correspond to a number of microseconds since some (unspecified) period in time. Timestamps
  /// are monotonically non-decreasing.
  void onArmUnsync(MyoPtr device, uint64_t timestamp) override;

  /// Called when a paired Myo becomes unlocked.
  /// @param device The Myo for this event.
  /// @param timestamp The timestamp of when the event is received by the SDK. Timestamps are 64 bit unsigned
  /// integers that correspond to a number of microseconds since some (unspecified) period in time. Timestamps
  /// are monotonically non-decreasing.
  void onUnlock(MyoPtr device, uint64_t timestamp) override;

  /// Called when a paired Myo becomes locked.
  /// @param device The Myo for this event.
  /// @param timestamp The timestamp of when the event is received by the SDK. Timestamps are 64 bit unsigned
  /// integers that correspond to a number of microseconds since some (unspecified) period in time. Timestamps
  /// are monotonically non-decreasing.
  void onLock(MyoPtr device, uint64_t timestamp) override;

  /// Called when a paired Myo has provided a new pose.
  /// @param device The Myo for this event.
  /// @param timestamp The timestamp of when the event is received by the SDK. Timestamps are 64 bit unsigned
  /// integers that correspond to a number of microseconds since some (unspecified) period in time. Timestamps
  /// are monotonically non-decreasing.
  /// @param pose The identified Pose of \a myo.
  void onPose(MyoPtr device, uint64_t timestamp, myo::Pose pose) override;

  /// Called when a paired Myo has provided new orientation data.
  /// @param device The Myo for this event.
  /// @param timestamp The timestamp of when the event is received by the SDK. Timestamps are 64 bit unsigned
  /// integers that correspond to a number of microseconds since some (unspecified) period in time. Timestamps
  /// are monotonically non-decreasing.
  /// @param rotation The orientation data of \a myo, as a Quaternion.
  void onOrientationData(MyoPtr device, uint64_t timestamp, const myo::Quaternion<float>& rotation) override;

  /// Called when a paired Myo has provided new accelerometer data in units of g.
  /// @param device The Myo for this event.
  /// @param timestamp The timestamp of when the event is received by the SDK. Timestamps are 64 bit unsigned
  /// integers that correspond to a number of microseconds since some (unspecified) period in time. Timestamps
  /// are monotonically non-decreasing.
  /// @param accel The accelerometer data of \a myo, in units of g.
  void onAccelerometerData(MyoPtr device, uint64_t timestamp, const myo::Vector3<float>& accel) override;

  /// Called when a paired Myo has provided new gyroscope data in units of deg/s.
  /// @param device The Myo for this event.
  /// @param timestamp The timestamp of when the event is received by the SDK. Timestamps are 64 bit unsigned
  /// integers that correspond to a number of microseconds since some (unspecified) period in time. Timestamps
  /// are monotonically non-decreasing.
  /// @param gyro The gyroscope data of \a myo, in units of deg/s.
  void onGyroscopeData(MyoPtr device, uint64_t timestamp, const myo::Vector3<float>& gyro) override;

  /// Called when a paired Myo has provided a new RSSI value.
  /// @param device The Myo for this event.
  /// @param timestamp The timestamp of when the event is received by the SDK. Timestamps are 64 bit unsigned
  /// integers that correspond to a number of microseconds since some (unspecified) period in time. Timestamps
  /// are monotonically non-decreasing.
  /// @param rssi The RSSI (received signal strength indication) of \a myo.
  /// @see Myo::requestRssi() to request an RSSI value from the Myo.
  void onRssi(MyoPtr device, uint64_t timestamp, int8_t rssi) override;

  /// Called when a paired Myo receives an battery level update.
  /// Updates occur when the battery level changes and when the battery level is explicitly requested.
  /// @param device The Myo for this event.
  /// @param timestamp The timestamp of when the event is received by the SDK. Timestamps are 64 bit unsigned
  /// integers that correspond to a number of microseconds since some (unspecified) period in time. Timestamps
  /// are monotonically non-decreasing.
  /// @param level battery level reported by the myo the value is a number from 0 to 100 representing the percentage
  /// of battery life remaining.
  void onBatteryLevelReceived(MyoPtr device, uint64_t timestamp, uint8_t level) override;

  /// Called when a paired Myo has provided new EMG data.
  /// @param device The Myo for this event.
  /// @param timestamp The timestamp of when the event is received by the SDK. Timestamps are 64 bit unsigned
  /// integers that correspond to a number of microseconds since some (unspecified) period in time. Timestamps
  /// are monotonically non-decreasing.
  /// @param emg An array of 8 elements, each corresponding to one sensor.
  void onEmgData(MyoPtr device, uint64_t timestamp, const int8_t* emg) override;

  /// Called when the warmup period for a Myo has completed.
  /// @param device The Myo for this event.
  /// @param timestamp The timestamp of when the event is received by the SDK. Timestamps are 64 bit unsigned
  /// integers that correspond to a number of microseconds since some (unspecified) period in time. Timestamps
  /// are monotonically non-decreasing.
  /// @param warmupResult The warmup result of \a myo.
  void onWarmupCompleted(MyoPtr device, uint64_t timestamp, myo::WarmupResult warmupResult) override;

private:
  const DeviceDataPaths& devicePaths(MyoPtr device) {
    const auto& state = _devices[device];
    return state.paths;
  }

  osc::OutboundPacketStream beginMessage(const std::string& path);

  void send(const osc::OutboundPacketStream& p);

  template<typename T>
  void send(const std::string& path, T val) {
    send(beginMessage(path) << val << osc::EndMessage);
  }

  template<typename T>
  void sendMessage(const std::string& path, T val) {
    send(path, val);
    _logger.verboseMessage(path, val);
  }
  void sendMessage(const std::array<std::string, emgLength>& path,
                   const int8_t* vals);
  void sendMessage(const std::array<std::string, 3>& path,
                   myo::Vector3<float> vec);
  void sendMessage(const std::array<std::string, 4>& path,
                   myo::Quaternion<float> quat);

  template<
    typename T,
    typename = std::enable_if_t<std::is_enum<T>::value>>
  void sendEnumMessage(const std::string& path, T val) {
    sendMessage(path, static_cast<std::int8_t>(val));
  }

  DeviceManager& _devices;
  const Settings& _settings;
  Logger& _logger;
  char _buffer[OUTPUT_BUFFER_SIZE];
  std::unique_ptr<UdpTransmitSocket> _socket;
};

