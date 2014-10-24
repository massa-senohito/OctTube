#include "stdafx.h"
#include "DebugLogger.h"
#define recast reinterpret_cast
void handleSocketError(int err){
  switch (err) {
  case WSABASEERR:
    break;
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
    printf("バッファのポインタが小さいか、無効です\n");
    break;
  case WSAECONNREFUSED:
    printf("要求が拒否されました\n");
    break;
  case WSAENOTSOCK:
    printf("socketに無効な値が渡されました");
  default:
    printf("other error: %d", err);
    //http://homepage1.nifty.com/yito/anhttpd/winsock_error.html
    break;
  }

}
struct ConnectInfo
{
  const char* IPAdress;
};

//1024ポートを使用して
void sockInit(){
#ifdef _SOCKDEBUG
  ver2= MAKEWORD(2, 0);
  error = WSAStartup ( ver2 , &wsaData );
  if ( error ){ WSACleanup(); }
  auto udp = IPPROTO_UDP;
  sock    = socket ( AF_INET , SOCK_DGRAM, 0);
  addr    = new SOCKADDR_IN;
  auto ht          = htons ( 1024 );
  addr->sin_port   = ht;
  addr->sin_family = AF_INET;
  addr->sin_addr.S_un.S_addr = inet_addr("127.0.0.1");;
#endif
  //error = bind( sock , recast<sockaddr*>( addr ), sizeof( addr ));
  //if (error==-1) handleSocketError( WSAGetLastError());
}
void DebugLogger::sockExit(SOCKET s)
{

  error = closesocket(s);
  if (error==-1) handleSocketError( WSAGetLastError());

  error = WSACleanup();
  if (error==-1) handleSocketError( WSAGetLastError());
  delete addr;
}
int sendCI(const SOCKET s, const SOCKADDR_IN* addr, const char* str){

#ifdef _SOCKDEBUG
  auto len = strlen( str );

  error = sendto(s, str, len, 0, recast<const sockaddr*>( addr ),
    sizeof( addr ));
  if (error==-1) handleSocketError( WSAGetLastError());
  return error;
#else
  return 0;
#endif
}

int DebugLogger::GetError(){
  handleSocketError( error );
  return error;
}
int DebugLogger::Send(const char* str){
  return sendCI( sock , addr , str);
}
DebugLogger::DebugLogger()
{
  sockInit();
}
DebugLogger::~DebugLogger()
{
#ifdef _SOCKDEBUG
  sockExit(sock);
#endif
}
