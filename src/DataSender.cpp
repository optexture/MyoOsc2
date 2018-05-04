//
//  DataSender.cpp
//  MyoOsc2
//

#include "DataSender.h"
#include <iomanip>

DataSender::DataSender(DeviceManager& devices,
                       const Settings& settings)
: _devices(devices)
, _settings(settings)
, _socket(std::make_unique<UdpTransmitSocket>(IpEndpointName(settings.hostname.c_str(), settings.port))) {}

static void logPath(const std::string& path) {
  std::cout << std::setw(20) << std::setfill(' ') << std::left << (path + ":");
}

static void logVal(float val) {
  std::cout << "  " << std::setw(10) << std::right << std::setprecision(2) << val;
}

static void logVal(int8_t val) {
  std::cout << "  " << std::setw(10) << std::right << (int)val;
}

static void logVector(const myo::Vector3<float>& vec) {
  logVal(vec.x());
  logVal(vec.y());
  logVal(vec.z());
}

static void logQuaterion(const myo::Quaternion<float>& quat) {
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

void DataSender::sendMessage(const std::string& path, int8_t val) {
  send(beginMessage(path) << val << osc::EndMessage);
  if (_settings.logging) {
    logPath(path);
    logVal(val);
    std::cout << std::endl;
  }
}

void DataSender::sendMessage(const std::string& path, const int8_t* vals, int count) {
  auto p = beginMessage(path);
  for (int i = 0; i < count; ++i) {
    p << vals[i];
  }
  send(p << osc::EndMessage);
  if (_settings.logging) {
    logPath(path);
    for (int i = 0; i < count; ++i) {
      logVal(vals[i]);
    }
    std::cout << std::endl;
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

void DataSender::sendMessage(const std::string& path, myo::Vector3<float> vec) {
  send(beginMessage(path)
       << vec.x() << vec.y() << vec.z() << osc::EndMessage);
  if (_settings.logging) {
    logPath(path);
    logVector(vec);
    std::cout << std::endl;
  }
}

void DataSender::sendMessage(const std::string& path, myo::Vector3<float> vec1, myo::Vector3<float> vec2) {
  send(beginMessage(path)
       << vec1.x() << vec1.y() << vec1.z()
       << vec2.x() << vec2.y() << vec2.z() << osc::EndMessage);
  if (_settings.logging) {
    logPath(path);
    logVector(vec1);
    logVector(vec2);
    std::cout << std::endl;
  }
}

void DataSender::sendMessage(const std::string& path, myo::Quaternion<float> quat) {
  send(beginMessage(path)
       << quat.x() << quat.y() << quat.z() << quat.w() << osc::EndMessage);
  if (_settings.logging) {
    logPath(path);
    logQuaterion(quat);
    std::cout << std::endl;
  }
}
