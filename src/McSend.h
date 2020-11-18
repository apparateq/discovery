
#pragma once

#include <sys/types.h>          // for type definitions
#include <sys/socket.h>         // for socket API function calls
#include <netinet/in.h>         // for address structs
#include <arpa/inet.h>          // for sockaddr_in
#include <stdio.h>              // for printf()
#include <stdlib.h>             // for atoi()
#include <unistd.h>             // for close()
#include <string>


class McSend {
public:
  McSend(std::string IpAddress, int Port);
  ~McSend();

  int sendData(char * TxBuffer, int TxSize);
  int receiveData(char * RxBuffer, int MaxRxLen);

private:
  const int MaxLen{1400};         // maximum string size to send
  int mSocket{-1};                // socket file descriptor
  struct sockaddr_in mMcAddr;     // socket address structure
  unsigned char mMcTTL{1};        // time to live (hop count)
};
