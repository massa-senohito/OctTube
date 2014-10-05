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
    printf("WSA�V�X�e�����l�b�g���[�N�ւ̐ڑ��������ł��Ă��܂���\n");
    break;
  case WSAVERNOTSUPPORTED:
    printf("WSAVERNOTSUPPORTED\n");
    break;
  case WSAEINPROGRESS:
    printf("WSA�u���b�L���O���삪���s���ł�\n");
    break;
  case WSAEPROCLIM:
    printf("WSA�͂���ȏ�̃v���Z�X���Ǘ��ł��܂���\n");
    break;
  case WSAEFAULT:
    printf("WSAData���L���̃|�C���^�ł͂���܂���\n");
    break;
  case WSAECONNREFUSED:
    printf("�v�������ۂ���܂���\n");
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
  //bind(s,addr,sizeof(addr)); ��M��
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
