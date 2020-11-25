#include "stdafx.h"
#include "protocol.h"

using namespace std;

enum eTHREAD { SEND = 0, RECV = 1, MAX = 2 };
enum eSCENE { LOBBY = 0, GAME = 1 };
constexpr int CLIENT_LIMITE = 2;

int client_table[2];
class hClientInfo {
public:
	int id;
	SOCKET s;
	SOCKADDR_IN addr;
	int addrlen;
	HANDLE hEvent[eTHREAD::MAX];
	DWORD dwThreadID[eTHREAD::MAX];
	eSCENE Scene = eSCENE::LOBBY;
	int lastpacket;

public:
	void close() {
		closesocket(s);
		s = NULL;
		ZeroMemory(&addr, sizeof(addr));
		CloseHandle(hEvent[eTHREAD::SEND]);
		CloseHandle(hEvent[eTHREAD::RECV]);
		dwThreadID[eTHREAD::SEND] = NULL;
		dwThreadID[eTHREAD::RECV] = NULL;
		client_table[id] = NULL;
	}
};
hClientInfo cInfo[CLIENT_LIMITE];
int client_cur = 0;

int type;

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

DWORD WINAPI SendThread(LPVOID arg) {
	
	DWORD retval;
	char buffer = ' ';
	auto threadID = GetCurrentThreadId();
	hClientInfo* client = static_cast<hClientInfo*>(arg);

	cout << "SENDTHREAD" << threadID << endl;
	while (1) {
		WaitForSingleObject(client->hEvent[eTHREAD::RECV], INFINITE);
		switch (client->lastpacket) {
		case cs_login:
			sc_packet_login scpl;
			scpl.id = client->id;
			scpl.size = sizeof(scpl);
			scpl.type = sc_login;
			retval = send(client->s, reinterpret_cast<char*>(&scpl), sizeof(scpl), 0);
			break;
		case CS_2:
			cout << threadID << "send - cs2" << endl;
			buffer = '2';
			break;
		case CS_3:
			cout << threadID << "send - cs3" << endl;
			buffer = '3';
			break;
		case CS_4:
			cout << threadID << "send - cs4" << endl;
			buffer = '4';
			break;
		}
		SetEvent(client->hEvent[eTHREAD::SEND]);
	}
	client->close();
	return 0;
}

DWORD WINAPI RecvThread(LPVOID arg) {

	DWORD retval;
	char buffer[MAXBUFFER];
	auto threadID = GetCurrentThreadId();

	cout << "RECVTHREAD" << threadID << endl;
	hClientInfo* client = static_cast<hClientInfo*>(arg);
	while (1) {
		WaitForSingleObject(client->hEvent[eTHREAD::SEND], INFINITE);
		ZeroMemory(&buffer, sizeof(buffer));
		retval = recv(client->s, buffer, 10, 0);
		if (retval == SOCKET_ERROR)  err_display("recv()");
		else if (retval == 0) return 0;
		switch (buffer[1]) {
		case cs_login:
			client->lastpacket = cs_login;
			break;
		case CS_2:
			type = CS_2;
			cout << threadID << "get packet - CS2" << endl;
			cout << buffer[0] << endl;
			cout << buffer[1] << endl;
			cout << buffer[2] << endl;
			cout << buffer[3] << endl;
			break;
		case CS_3:
			type = CS_3;
			cout << threadID << "get packet - CS3" << endl;
			cout << buffer[0] << endl;
			cout << buffer[1] << endl;
			cout << buffer[2] << endl;
			cout << buffer[3] << endl;
			break;
		case CS_4:
			type = CS_4;
			cout << threadID << "get packet - CS4" << endl;
			cout << buffer[0] << endl;
			cout << buffer[1] << endl;
			cout << buffer[2] << endl;
			cout << buffer[3] << endl;
			break;
		}
		SetEvent(client->hEvent[eTHREAD::RECV]);
	}
	client->close();
	return 0;
}

int main(int argc, char* argv[]) {
	memset(&client_table, 0, sizeof(client_table));
	int retval;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa)) return -1;

	SOCKET sock_listen = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_listen == INVALID_SOCKET) err_display("socket()");

	SOCKADDR_IN addr_server;
	ZeroMemory(&addr_server, sizeof(addr_server));
	addr_server.sin_family = AF_INET;
	addr_server.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addr_server.sin_port = htons(SERVERPORT);
	retval = bind(sock_listen, (sockaddr*)&addr_server, sizeof(addr_server));
	if (retval == SOCKET_ERROR) err_display("bind()");

	retval = listen(sock_listen, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_display("listen()");
	int sockopt = true;
	setsockopt(sock_listen, IPPROTO_TCP, TCP_NODELAY, (const char*)&sockopt, sizeof(sockopt));
	
	HANDLE hThread;
	
	while (true) {
		if (client_table[0] == NULL) client_cur = 0;
		else if (client_table[1] == NULL) client_cur = 1;
		else continue;

		cInfo[client_cur].addrlen = sizeof(cInfo[client_cur].addr);
		cInfo[client_cur].s = accept(sock_listen, (sockaddr*)&cInfo[client_cur].addr, &cInfo[client_cur].addrlen);

		cInfo[client_cur].id = client_cur;
		client_table[client_cur] = 1;
		if (cInfo[client_cur].s == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}
		cInfo[client_cur].hEvent[eTHREAD::RECV]= CreateEvent(NULL, false, false, NULL);
		cInfo[client_cur].hEvent[eTHREAD::SEND]= CreateEvent(NULL, false, true, NULL);

		cout << inet_ntoa(cInfo[client_cur].addr.sin_addr) << " " << ntohs(cInfo[client_cur].addr.sin_port) << endl;

		hThread = CreateThread(NULL, 0, SendThread, &cInfo[client_cur], 0, &cInfo[client_cur].dwThreadID[eTHREAD::SEND]);
		if (hThread == NULL) { closesocket(cInfo[client_cur].s); }
		else {
			SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);
			CloseHandle(hThread);
		}
		hThread = CreateThread(NULL, 0, RecvThread, &cInfo[client_cur], 0, &cInfo[client_cur].dwThreadID[eTHREAD::RECV]);
		if (hThread == NULL) { closesocket(cInfo[client_cur].s); }
		else {
			SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);
			CloseHandle(hThread);
		}

	}

	closesocket(sock_listen);
	WSACleanup();
	return 0;
}