

#pragma once

#include <Device.h>
#include <McReceive.h>
#include <Protocol.h>

class Client {
public:
  Client(std::string IpAddr, int port);

  /// \brief contniuously receive message, handle requests and send replies
  void mainThread();

  /// \brief wrapper for McReceive
  int receiveData() {
    return mcreceive->receiveData((char *)&request, sizeof(Request::Message));
  }


private:
  /// \brief handle received message and indicate if reply is to be sent
  bool handleRequest(int Size);

  /// \brief sends a reply after a random delay
  void delayedTransmit();

  McReceive * mcreceive;
  Device device;
  Request::Message request;
  //
  uint16_t Status{Request::StatusBooting};
  char IpAddr[Request::IpAddrLen];
  char Configuration[Request::ConfigLen];
};
