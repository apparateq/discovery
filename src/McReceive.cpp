//******************************************************************
// Copyright (C) 2011-2020 Morten Jagd Christensen
//******************************************************************

#include <McReceive.h>
#include <assert.h>
#include <arpa/inet.h>          // for inet_addr()
#include <sys/socket.h>         // for socket API calls
#include <unistd.h>             // for close()
#include <string.h>             // memset


McReceive::McReceive(std::string IpAddr, int Port) {
  if ((mSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
      perror("socket() failed");
      exit(1);
  }

  int FlagOn = 1; // socket option flag
  if ((setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, &FlagOn, sizeof(FlagOn))) < 0) {
      perror("setsockopt(SO_REUSEADDR) failed");
      exit(1);
  }

  if ((setsockopt(mSocket, SOL_SOCKET, SO_REUSEPORT, &FlagOn, sizeof(FlagOn))) < 0) {
      perror("setsockopt(SO_REUSEPORT) failed");
      exit(1);
  }

  // construct a multicast address structure
  memset(&mMcAddr, 0, sizeof(mMcAddr));
  mMcAddr.sin_family = AF_INET;
  mMcAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  mMcAddr.sin_port = htons(Port);

  // bind to multicast address to socket
  if ((bind(mSocket, (struct sockaddr *)&mMcAddr, sizeof(mMcAddr))) < 0) {
      perror("bind() failed");
      exit(1);
  }

  // construct an IGMP join request structure
  mMcReq.imr_multiaddr.s_addr = inet_addr(IpAddr.c_str());
  mMcReq.imr_interface.s_addr = htonl(INADDR_ANY);

  // send an ADD MEMBERSHIP message via setsockopt
  if ((setsockopt(mSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *)&mMcReq, sizeof(mMcReq))) < 0) {
      perror("setsockopt(IP_ADD_MEMBERSHIP) failed");
      exit(1);
  }
}

McReceive::~McReceive() {
  close(mSocket);
}


int McReceive::replyTo(char * TxBuffer, int MaxTxSize) {
   int Result = sendto(mSocket, TxBuffer, MaxTxSize, 0, (sockaddr*)&mRemoteAddr, sizeof(mRemoteAddr));
   //printf("%d bytes sent\n", Result);
   return Result;
}


int McReceive::receiveData(char * RxBuffer, int MaxRxSize) {
    unsigned int FromLength{sizeof(mRemoteAddr)};   // source addr length
    int RecvLength;            // length of string received
    memset(&mRemoteAddr, 0, sizeof(mRemoteAddr));

    if ((RecvLength = recvfrom(mSocket, RxBuffer, MaxRxSize,
          0, (struct sockaddr *)&mRemoteAddr, &FromLength)) < 0) {
        perror("recvfrom() failed");
        return -1;
    }

    char ipstring[20];
    const char * remote = inet_ntop(AF_INET, &(mRemoteAddr.sin_addr), ipstring, INET_ADDRSTRLEN);
    assert(remote != NULL);

    //printf("client: received: %s (%d bytes)  from %s:%d\n", RxBuffer, RecvLength,
    //        ipstring, ntohs(mRemoteAddr.sin_port));

    return RecvLength;
}
