#pragma once

#include  <winsock2.h>
class DebugLogger
{
  
  void sockExit(SOCKET s);
SOCKET sock;
SOCKADDR_IN* addr;
WSADATA wsaData;
WORD ver2 ;
int error;
public:
  static bool RenderSimple;
  DebugLogger();
  int GetError();
  int Send(const char*);
  ~DebugLogger();
};

