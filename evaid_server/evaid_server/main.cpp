#include "stdafx.h"
#include "protocol.h"

using namespace std;

enum { THREADLIMIT = 4 };
static DWORD dwThreadID[THREADLIMIT];
int threadCnt = 0;

void err_display(const string& msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	::cout << "[" << msg << "]" << GetLastError();
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

int recvp(SOCKET s, char* buf, int flags) {
	int packetsize;
	char* ptr = buf;
	packetsize = recvn(s, ptr, sizeof(short), flags);
	return recvn(s, ptr + sizeof(short), packetsize, flags);
}

DWORD WINAPI SendThread(LPVOID arg) {
	SOCKET sock_client = (SOCKET)arg;
	SOCKADDR_IN addr_client;
	DWORD retval;
	int addrlen = sizeof(addr_client);
	getpeername(sock_client, (sockaddr*)&addr_client, &addrlen);

	char buffer[MAXBUFFER];

}

DWORD WINAPI RecvThread(LPVOID arg) {
	SOCKET sock_client = (SOCKET)arg;
	SOCKADDR_IN addr_client;
	DWORD retval;
	int addrlen = sizeof(addr_client);
	getpeername(sock_client, (sockaddr*)&addr_client, &addrlen);
	char buffer[MAXBUFFER];
	auto threadID = GetCurrentThreadId();

	while (1) {
		retval = recvp(sock_client, buffer, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		} else if (retval == 0) break;

		switch (buffer[1]) {
		case CS_1:
			cout << "get packet - CS1" << endl;
			break;
		case CS_2:
			cout << "get packet - CS2" << endl;
			break;
		case CS_3:
			cout << "get packet - CS3" << endl;
			break;
		case CS_4:
			cout << "get packet - CS4" << endl;
			break;
		}
	}
}

int main(int argc, char* argv[]) {
	int retval;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa)) return -1;

	SOCKET sock_listen = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_listen == INVALID_SOCKET) err_display("socket()");

	SOCKADDR_IN addr_server;
	ZeroMemory(&addr_server, sizeof(addr_server));
	addr_server.sin_family = AF_INET;
	addr_server.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addr_server.sin_port = htonl(SERVERPORT);
	retval = bind(sock_listen, (sockaddr*)&addr_server, sizeof(addr_server));
	if (retval == SOCKET_ERROR) err_display("bind()");

	retval = listen(sock_listen, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_display("listen()");

	SOCKET sock_client;
	SOCKADDR_IN addr_client;
	int addrlen;
	
	HANDLE hThread;

	while (true) {
		addrlen = sizeof(addr_client);
		sock_client = accept(sock_listen, (sockaddr*)&addr_client, &addrlen);
		if (sock_client == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}
		cout << inet_ntoa(addr_client.sin_addr) << ntohs(addr_client.sin_port) << endl;
		hThread = CreateThread(NULL, 0, SendThread, (LPVOID)sock_client, 0, &dwThreadID[threadCnt]);
		if (hThread == NULL) { closesocket(sock_client); }
		else {
			SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);
			CloseHandle(hThread);
		}
		threadCnt = (threadCnt + 1) % THREADLIMIT;
		hThread = CreateThread(NULL, 0, RecvThread, (LPVOID)sock_client, 0, &dwThreadID[threadCnt]);
		if (hThread == NULL) { closesocket(sock_client); }
		else {
			SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);
			CloseHandle(hThread);
		}
		threadCnt = (threadCnt + 1) % THREADLIMIT;
	}

	closesocket(sock_listen);
	WSACleanup();
	return 0;
}