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
		
		Protocol_TABLE table;
		for (int i = 0; i < 20; ++i) {
			for (int j = 0; j < 10; ++j) {
				table.t[i][j] = i;
			}
		}
		int id = 0;
		cs_packet_user cspu;
		ZeroMemory(&cspu, sizeof(cspu));
		cspu.size = sizeof(cspu);
		cspu.type = cs_user;
		memcpy(&cspu.table, reinterpret_cast<char*>(&table), sizeof(table));
		cspu.pos[0] = 10;
		cspu.pos[1] = 20;
		cspu.skillGauge = 11;
		cspu.skillActive = false;
		cspu.nextBlock = 1;

		retval = send(sock, reinterpret_cast<char*>(&cspu), sizeof(cspu), 0);

		sc_packet_user scpu;
		ZeroMemory(&scpu, sizeof(scpu));
		retval = recv(sock, reinterpret_cast<char*>(&scpu), sizeof(scpu), 0);
		cout << scpu.pos[id][0] << " " << scpu.pos[id][1] << endl;
		cout << static_cast<int>(scpu.skillGauge[id]) << endl;
		cout << static_cast<int>(scpu.skillActive[id]) << endl;
		cout << static_cast<int>(scpu.nextBlock[id]) << endl;
		system("pause");
		flag = false;
	}

	//소켓종료, 받은 데이터 버퍼 삭제, WSA종료
	closesocket(sock);
	WSACleanup();
}