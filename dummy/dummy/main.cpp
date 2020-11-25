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

	//�ݺ� ���� flag
	bool flag = true;
	//���� ������ ���� Ŀ��
	int fileCur = 0;
	while (flag) {
		//���ۿ� ����
		char sendBuffer[MAXBUFFER];

		cs_packet_3 cs3;
		cs3.size = 4;
		cs3.data_1 = 'b';
		cs3.data_2 = 'c';
		cs3.type = CS_3;

		//������ ���� ����: ũ��, ������ũ��, �̸�
		retval = send(sock, reinterpret_cast<char*>(&cs3), cs3.size, 0);
		if (retval == SOCKET_ERROR) err_display("send()");
		else cout << "[TCP client]" << retval << "����" << endl;

		char recvBuffer;
		ZeroMemory(&recvBuffer, sizeof(recvBuffer));
		retval = recv(sock, &recvBuffer, sizeof(recvBuffer), 0);
		cout << recvBuffer << endl;

		cs_packet_2 cs2;
		cs2.size = 3;
		cs2.data = 'b';
		cs2.type = CS_2;

		//������ ���� ����: ũ��, ������ũ��, �̸�
		retval = send(sock, reinterpret_cast<char*>(&cs2), cs2.size, 0);
		if (retval == SOCKET_ERROR) err_display("send()");
		else cout << "[TCP client]" << retval << "����" << endl;

		ZeroMemory(&recvBuffer, sizeof(recvBuffer));
		retval = recv(sock, &recvBuffer, sizeof(recvBuffer), 0);
		cout << recvBuffer << endl;

		cs_packet_1 cs1;
		cs1.size = 3;
		cs1.data = 'b';
		cs1.type = CS_1;

		//������ ���� ����: ũ��, ������ũ��, �̸�
		retval = send(sock, reinterpret_cast<char*>(&cs1), cs1.size, 0);
		if (retval == SOCKET_ERROR) err_display("send()");
		else cout << "[TCP client]" << retval << "����" << endl;

		ZeroMemory(&recvBuffer, sizeof(recvBuffer));
		retval = recv(sock, &recvBuffer, sizeof(recvBuffer), 0);
		cout << recvBuffer << endl;
		flag = false;
	}

	//��������, ���� ������ ���� ����, WSA����
	closesocket(sock);
	WSACleanup();
}