
#include <Server.h>
#include <Device.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h> // memset


void Server::sendReadRequest() {
  memset(&request, 0, sizeof(request));
  request.Cookie = Request::ApparateqCookie;
  request.Version = 0x0001;
  request.SeqNo = SeqNo;
  request.Command = Request::CmdReadRequest;
  request.Destination = Request::DestAll;
  mcsend->sendData((char *)&request, sizeof(request));
}

void Server::sendConfigRequest(uint32_t DestSerialNo) {
  memset(&request, 0, sizeof(request));
  request.Cookie = Request::ApparateqCookie;
  request.Version = 0x0001;
  request.SeqNo = SeqNo;
  request.Command = Request::CmdWriteConfig;
  request.Destination = DestSerialNo;
  // memcpy() // Add config data here
  mcsend->sendData((char *)&request, sizeof(request));
}

void Server::sendAddrRequest(uint32_t DestSerialNo, std::string DestIpAddr) {
  if (DestSerialNo == Request::DestAll) {
    printf("Ip address command not allowed for 'ALL'\n");
    return;
  }
  if (DestIpAddr.size() < 7) { // E.g. "1.1.1.1" shortest valid ip address
    printf("Invalid ip address (too short)\n");
    return;
  }

  memset(&request, 0, sizeof(request));
  request.Cookie = Request::ApparateqCookie;
  request.Version = 0x0001;
  request.SeqNo = SeqNo;
  request.Command = Request::CmdAddressSet;
  request.Destination = DestSerialNo;
  memcpy(request.IpAddr, DestIpAddr.c_str(), DestIpAddr.size());
  mcsend->sendData((char *)&request, sizeof(request));
}

bool Server::handleReadReply() {
  if (request.SeqNo != SeqNo) {
    printf("Wrong sequence number\n");
    return false;
  }
  if (request.Command != Request::CmdReadReply) {
    printf("Not ReadReply\n");
    return false;
  }
  return true;
}


void Server::discoverDevices() {
  sendReadRequest();

  clients.clear();

  auto start = std::chrono::steady_clock::now();
  auto end = std::chrono::steady_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  do {
    if (receiveData() > 0) {
      bool ReplyGood = handleReadReply();
      if (ReplyGood) {
        clients.push_back(request);
      }
    }

    end = std::chrono::steady_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  } while (elapsed.count() <= 5000000);

  if (clients.size() == 0) {
    printf("No devices found\n");
  } else {
    printf("Found the following devices:\n");
    printf("Id     Name   Serial no     Status       Ip addr\n");
    printf("---------------------------------------------------\n");
    for (auto & client : clients) {
      printf("%2d %10s 0x%08x   %8s      %s\n", client.ProductId,
         Device::serialNo2Name(client.SerialNo).c_str(), client.SerialNo,
         Request::Status(client.Status).c_str(), client.IpAddr);
    }
  }
  SeqNo++;
}


void Server::setDeviceAddress() {
  std::string SerialNo;
  std::string IpAddress;

  printf("Enter SerialNo: ");
  std::cin >> SerialNo;
  printf("Enter IpAddr: ");
  std::cin >> IpAddress;
  printf("SerialNo: %s / 0x%08x\n", SerialNo.c_str(), std::stoi(SerialNo, nullptr, 16));
  sendAddrRequest(std::stoi(SerialNo, nullptr, 16), IpAddress);
}

void Server::setDeviceConfig() {
  std::string SerialNo;

  printf("Enter SerialNo: ");
  std::cin >> SerialNo;
  sendConfigRequest(std::stoi(SerialNo, nullptr, 16));
}

void Server::printMenu() {
  printf("Commands\n");
  printf("1) Discover devices\n");
  printf("2) Set IP address of device\n");
  printf("3) Configure device\n");
  printf("h) Print this menu\n");
  printf("q) Quit\n");
}

void Server::commandLoop() {
  char Command;
  while (true) {
    printf("Enter command: ");
    std::cin >> Command;
    switch (Command) {
      case '1':
        discoverDevices();
        break;
      case '2':
        setDeviceAddress();
        break;
      case '3':
        setDeviceConfig();
        break;
      case 'h':
        printMenu();
        break;
      case 'q':
        return;
        break;
    }
  }
}

void Server::mainThread() {

  printMenu();
  commandLoop();
}

Server::Server(std::string IpAddr, int Port) {
  mcsend = new McSend(IpAddr, Port);
}
