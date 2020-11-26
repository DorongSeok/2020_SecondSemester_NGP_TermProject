#include <iostream>
#include <fstream>
#include <WinSock2.h>
#include <Windows.h>
#include <CommCtrl.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)
#include "..\..\evaid_server\evaid_server\protocol.h"
using namespace std;

#define MAX_LOADSTRING 100

void err_display(const string& msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	cout << "[" << msg << "]" << GetLastError();
	LocalFree(lpMsgBuf);
}

int recvn(SOCKET s, char* buf, int len, int flags) {
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR) return SOCKET_ERROR;
		else if (received == 0) break;
		left -= received;
		ptr += received;
	}
	return (len - left);
}

int main(int argc, char* argv[]) {
	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa)) return -1;

	//socket();
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_display("socket()");

	//connect();
	sockaddr_in serverAddress;
	ZeroMemory(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.S_un.S_addr = inet_addr(SERVERIP);
	serverAddress.sin_port = htons(SERVERPORT);
	retval = connect(sock, (sockaddr*)&serverAddress, sizeof(serverAddress));
	if (retval == SOCKET_ERROR) err_display("connect()");

	//반복 종료 flag
	bool flag = true;
	//받은 데이터 버퍼 커서
	int fileCur = 0;
	while (flag) {
		//cs_packet_login cspl;
		//ZeroMemory(&cspl, sizeof(cspl));
		//cspl.type = cs_login;
		//cspl.size = sizeof(cspl);
		//retval = send(sock, reinterpret_cast<char*>(&cspl), cspl.size, 0);

		//sc_packet_login scpl;
		//ZeroMemory(&scpl, sizeof(scpl));
		//retval = recv(sock, reinterpret_cast<char*>(&scpl), sizeof(scpl), 0);
		//int id = static_cast<int>(scpl.id);
		//cout << id << endl;
		//
		//char tmp;
		//cin >> tmp;

		//cs_packet_ready cspr;
		//ZeroMemory(&cspr, sizeof(cspr));
		//cspr.type = cs_ready;
		//cspr.size = sizeof(cspr);
		//cspr.id = id;
		//retval = send(sock, reinterpret_cast<char*>(&cspr), cspr.size, 0);


		//전송용 버퍼
		char sendBuffer[MAXBUFFER];
		cs_packet_3 cs3;
		cs3.size = 3;
		cs3.data_1 = 'b';
		cs3.data_2 = 'c';
		cs3.type = CS_3;

		//데이터 정보 전달: 크기, 데이터크기, 이름
		retval = send(sock, reinterpret_cast<char*>(&cs3), cs3.size, 0);
		if (retval == SOCKET_ERROR) err_display("send()");
		else cout << "[TCP client]" << retval << "전송" << endl;

		char recvBuffer;
		ZeroMemory(&recvBuffer, sizeof(recvBuffer));
		retval = recv(sock, &recvBuffer, sizeof(recvBuffer), 0);
		cout << recvBuffer << endl;
		flag = false;
	}

	//소켓종료, 받은 데이터 버퍼 삭제, WSA종료
	closesocket(sock);
	WSACleanup();
}