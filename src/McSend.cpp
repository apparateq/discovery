//*******************************************************************
// Copyright (C) 2011-2020 Morten Jagd Christensen
//*******************************************************************

#include <McSend.h>
#include <string.h> // memset

McSend::McSend(std::string IpAddr, int Port) {
  if ((mSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
      perror("socket() failed");
      exit(1);
  }

  struct timeval read_timeout;
  read_timeout.tv_sec = 0;
  read_timeout.tv_usec = 100000;
  setsockopt(mSocket, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);

  if ((setsockopt(mSocket, IPPROTO_IP, IP_MULTICAST_TTL, (void *)&mMcTTL, sizeof(mMcTTL))) < 0) {
      perror("setsockopt(IP_MULTICAST_TTL) failed");
      exit(1);
  }

  memset(&mMcAddr, 0, sizeof(mMcAddr));
  mMcAddr.sin_family      = AF_INET;
  mMcAddr.sin_addr.s_addr = inet_addr(IpAddr.c_str());
  mMcAddr.sin_port        = htons(Port);
}

McSend::~McSend() {
  close(mSocket);
}

int McSend::sendData(char * TxBuffer, int TxSize) {
  // send string to multicast address */
  if ((sendto(mSocket, TxBuffer, TxSize, 0, (struct sockaddr *)&mMcAddr,
        sizeof(mMcAddr))) != TxSize){
    return 0;
  }
  return 1;
}

int McSend::receiveData(char * RxBuffer, int MaxRxLen) {
  return recv(mSocket, RxBuffer, MaxLen, 0);
}
