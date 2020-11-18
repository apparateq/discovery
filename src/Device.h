
#pragma once
#include <cinttypes>
#include <string>

class Device {
public:
  Device();

  static std::string serialNo2Name(uint32_t SerialNo);
  static uint32_t serialNo2Id(uint32_t SerialNo);

  uint32_t mSerialNo{0};
  uint32_t mProductId{0};
  std::string mProductName;
};
