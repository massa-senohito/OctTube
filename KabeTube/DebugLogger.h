#pragma once

#include  <winsock2.h>
#define cstring const char*
typedef std::string string;
#define strstr cstring, cstring
typedef std::map<strstr> LogData;
typedef const std::pair<strstr> LogContain;
class DebugLogger
{
  
  void sockExit(SOCKET s);
  SOCKET sock;
  SOCKADDR_IN* addr;
  WSADATA wsaData;
  WORD ver2 ;
  LogData& log;
  int error;
public:
  static bool RenderSimple;
  DebugLogger();
  void MessagePush(cstring,cstring);
  int ReadPushed(std::function<void(LogContain&)>);
  int GetError();
  int Send(const char*);
  ~DebugLogger();
};

