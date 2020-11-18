
#pragma once

namespace Request {

const int IpAddrLen{16};
const int DescLen{128};
const int ConfigLen{512};

const uint32_t ApparateqCookie{0x41505451}; // APTQ in ascii

const uint32_t DestAll{0xffffffff};  // server -> client

const uint32_t CmdReadRequest{0x01}; // server -> client
const uint32_t CmdReadReply{0x02};   // client -> server
const uint32_t CmdWriteConfig{0x03}; // server -> client
const uint32_t CmdWriteReply{0x04};  // client -> server
const uint32_t CmdAddressSet{0x05};  // server -> client

const uint16_t StatusBooting{0x01};  // ready to receive ip address
const uint16_t StatusBooted{0x02};   // have an ip address
const uint16_t StatusReady{0x03};    // configured, ready to operate
const uint16_t StatusRunning{0x04};  // running - if applicable

static std::string Status(uint16_t Status) {
  switch (Status) {
    case StatusBooting:
      return "Booting";
      break;
    case StatusBooted:
      return "Booted";
      break;
    case StatusReady:
      return "Ready";
      break;
    case StatusRunning:
      return "Running";
      break;
    default:
      return "Unknown";
      break;
  }
}

struct Message {
  uint32_t Cookie{ApparateqCookie}; // Apparateq cookie
  uint16_t Version{0x0001}; //
  uint16_t Command;         // TBD: readconfig, writeconfig
  uint32_t SeqNo;           // sequence number - should be echoed back
  uint32_t Destination;     // 0xffffffff for all devices or serial number

  uint32_t ProductId; // product id
  uint32_t SerialNo;  // unique serial number
  uint16_t Status;    // booting, booted, configured, running, ...
  char IpAddr[IpAddrLen];    // ip address of this device
  char Description[DescLen];     // text string describing what role this device has
  char Configuration[ConfigLen];     // memory map for configuration
} __attribute__((packed));

//
static_assert(sizeof(struct Message) == 682, "Request size mismatch");



}
