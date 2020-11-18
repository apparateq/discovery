

#pragma once

#include <McSend.h>
#include <Protocol.h>
#include <vector>

class Server {
public:
  Server(std::string IpAddress, int Port);

  void mainThread();
  void sendReadRequest();
  bool handleReadReply();

  void sendAddrRequest(uint32_t DestSerialNo, std::string DestIpAddr);
  void sendConfigRequest(uint32_t DestSerialNo);

  void commandLoop();
  void printMenu();
  void discoverDevices();
  void setDeviceAddress();
  void setDeviceConfig();

  int receiveData() {
    return mcsend->receiveData((char *)&request, sizeof(Request::Message));
  }

private:
  int SeqNo{1};
  McSend * mcsend;
  struct Request::Message request;
  std::vector<Request::Message> clients;
};
