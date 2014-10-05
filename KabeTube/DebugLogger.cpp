#include "stdafx.h"
#include "DebugLogger.h"
#include  <winsock2.h>

struct ConnectInfo
{
  const char* IPAdress;
};
void sockInit(){
  WSADATA wsaData;
  WORD ver2 = MAKEWORD(2, 0);
  WSAStartup(ver2, &wsaData);
}
void sockExit(){
  WSACleanup();
}
void handleSocketError(int err){
  switch (err) {
  case WSASYSNOTREADY:
    printf("WSAシステムがネットワークへの接続準備ができていません\n");
    break;
  case WSAVERNOTSUPPORTED:
    printf("WSAVERNOTSUPPORTED\n");
    break;
  case WSAEINPROGRESS:
    printf("WSAブロッキング操作が実行中です\n");
    break;
  case WSAEPROCLIM:
    printf("WSAはこれ以上のプロセスを管理できません\n");
    break;
  case WSAEFAULT:
    printf("WSADataが有効のポインタではありません\n");
    break;
  case WSAECONNREFUSED:
    printf("要求が拒否されました\n");
    break;
  default:
    printf("other error: %d", err);
    //http://homepage1.nifty.com/yito/anhttpd/winsock_error.html
    break;
  }

}
SOCKADDR_IN initFromConnectInfo(ConnectInfo* ci){
  SOCKADDR_IN addr;
  addr.sin_port = htons(12345);
  addr.sin_family = AF_INET;
  addr.sin_addr.S_un.S_addr = inet_addr
    (ci->IPAdress);
  return addr;
}
void sendCI(const SOCKET s, const SOCKADDR_IN* addr, const char* str){
  auto len=strlen(str);
  sendto(s, str, len, 0, reinterpret_cast<const struct sockaddr*>(addr), sizeof(addr));
}
void ConnectServer(ConnectInfo* ci){
  SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_UDP);
  SOCKADDR_IN addr = initFromConnectInfo(ci);
  //bind(s,addr,sizeof(addr)); 受信側
  sendCI(s, &addr, "111");
}

DebugLogger::DebugLogger()
{
  sockInit();
}
DebugLogger::~DebugLogger()
{
  sockExit();
}
