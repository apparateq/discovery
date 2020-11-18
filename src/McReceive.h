
#pragma once

#include <netinet/in.h>         // for address structs
#include <string>


class McReceive {
public:
  McReceive(std::string IpAddr, int Port);
  ~McReceive();

  /// \brief receive multicast data into buffer
  /// \return -1 on error, length of received data otherwise
  int receiveData(char * RxBuffer, int MaxRxSize);

  int replyTo(char * TxBuffer, int MaxTxSize);

private:
  const int MaxLen{1400};         // maximum receive string size
  int mSocket;                    // socket descriptor
  struct sockaddr_in mMcAddr;     // socket address structure
  struct sockaddr_in mRemoteAddr; // info about sender
  struct ip_mreq  mMcReq;         // multicast request structure
};
