
#include <Client.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


Client::Client(std::string McIpAddr, int Port) {
  mcreceive = new McReceive(McIpAddr, Port);
  snprintf((char *)&IpAddr, Request::IpAddrLen, "0.0.0.0");
}

void Client::mainThread() {
  while (true) {
    int Size = receiveData();
    bool SendReply = handleRequest(Size);
    if (SendReply) {
      delayedTransmit();
    }
  }
}

void Client::delayedTransmit() {
  int SleepTimeUS = (rand()*3000000.0)/RAND_MAX;
  usleep(SleepTimeUS);
  mcreceive->replyTo((char *)&request, sizeof(Request::Message));
}


bool Client::handleRequest(int Size) {
  if (request.Cookie != Request::ApparateqCookie) {
    printf("Not an Apparateq request, ignoring.\n");
    return false;
  }

  if (request.Version != 0x0001 ) {
    printf("Unsupported version: %d\n", request.Version);
    return false;
  }

  if (Size != sizeof(Request::Message)) {
    printf("Invalid size, expected %zu, got %d\n", sizeof(Request::Message), Size);
    return false;
  }

  //printf("SeqNo: 0x%04x\n", request.SeqNo);
  //printf("Destination: 0x%04x\n", request.Destination);
  if (request.Destination != 0xffffffff and request.Destination != device.mSerialNo) {
    printf("Message not destined for us\n");
    return false;
  }

  switch (request.Command) {
    case Request::CmdReadRequest:
      printf("Read request received, sending reply\n");
      request.Command = Request::CmdReadReply;
      request.ProductId = device.mProductId;
      request.SerialNo = device.mSerialNo;
      request.Status = Status;
      memcpy(request.IpAddr, IpAddr, Request::IpAddrLen);
      return true;
      break;

    case Request::CmdAddressSet:
      printf("Address set request received, setting ip\n");
      ///\todo validate IP address eventually
      memcpy(IpAddr, request.IpAddr, Request::IpAddrLen);
      Status = Request::StatusBooted;
      return false;
      break;

    case Request::CmdWriteConfig:
      printf("Configuration request received\n");
      memcpy(Configuration, request.Configuration, Request::ConfigLen);
      Status = Request::StatusReady;
      return false;
      break;

    default:
      return false;
  }
}
