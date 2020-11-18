

#include <Device.h>
#include <unistd.h>

Device::Device() {
  mSerialNo = getpid();
  mProductId = serialNo2Id(mSerialNo);
  mProductName = serialNo2Name(mSerialNo);

  printf("Product:    %s\n", mProductName.c_str());
  printf("Product ID: %d\n", mProductId);
  printf("Serial No:  0x%04x%04x\n", mProductId, mSerialNo);
}


uint32_t Device::serialNo2Id(uint32_t SerialNo) {
  return SerialNo % 10;
}

std::string Device::serialNo2Name(uint32_t SerialNo) {
  switch (serialNo2Id(SerialNo)) {
    case 0:
      return "unknown";
      break;
    case 1:
      return "AP2201";
      break;
    case 2:
      return "AP2202";
      break;
    case 3:
      return "AP2203";
      break;
    case 4:
      return "AP5712";
      break;
    case 5:
      return "AP5713";
      break;
    case 6:
      return "AP5714";
      break;
    case 7:
      return "AP3347";
      break;
    case 8:
      return "AP3447";
      break;
    case 9:
      return "AP3547";
      break;
    default:
    return "unknown";
    break;
  }
}
