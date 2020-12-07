#include "stdafx.h"
#include "protocol.h"

using std::cout;
using std::endl;


int client_table[2];

class hClientInfo {
public:
	int id;
	SOCKET s;
	SOCKADDR_IN addr;
	int addrlen;
	HANDLE hEvent[(int)eTHREAD::THREAD_MAX];
	DWORD dwThreadID[(int)eTHREAD::THREAD_MAX];
	eSCENE Scene;
	int lastpacket;
	// lobby
	bool isReady;

	// game
	Protocol_TABLE table;
	int pos[(int)ePosition::POS_MAX];
	BYTE skillGauge;
	bool skillActive;
	BYTE nextBlock;
	eHeroState state;
	bool getUser;
public:

	hClientInfo() {
		init();
	}
	~hClientInfo() {
		close();
	}

	void init() {
		id = NULLVAL;
		isReady = false;
		Scene = eSCENE::SCENE_LOBBY;
	}

	void regame() {
		ZeroMemory(&table, sizeof(table));
		ZeroMemory(&pos, sizeof(pos));
		skillGauge = 0;
		skillActive = false;
		nextBlock = (BYTE)eBlock::BLOCK_A;
		getUser = false;
	}

	void close() {
		closesocket(s);
		id = NULLVAL;
		s = NULL;
		ZeroMemory(&addr, sizeof(addr));
		CloseHandle(hEvent[(int)eTHREAD::THREAD_SEND]);
		CloseHandle(hEvent[(int)eTHREAD::THREAD_RECV]);
		dwThreadID[(int)eTHREAD::THREAD_SEND] = NULL;
		dwThreadID[(int)eTHREAD::THREAD_RECV] = NULL;
		client_table[id] = NULL;
	}
};

class Client {
public:
	hClientInfo info[(int)ePlayer::PLAYER_MAX];
	sc_packet_user scpu;

	int sendAll(char* buffer, int bufferSize, int flags) {
		int cnt = 0;
		for (int i = 0; i < (int)ePlayer::PLAYER_MAX; ++i) {
			if (info[i].id != NULLVAL) {
				++cnt;
				send(info[i].s, buffer, bufferSize, flags);
			}
		}
		return cnt;
	}

	bool getReadyAll() {
		if (info[(int)ePlayer::PLAYER_FIRST].isReady == true
			&& info[(int)ePlayer::PLAYER_SECOND].isReady == true) {
			info[(int)ePlayer::PLAYER_FIRST].isReady = false;
			info[(int)ePlayer::PLAYER_SECOND].isReady = false;
			return true;
		}
		return false;
	}

	void regame() {
		info[(int)ePlayer::PLAYER_FIRST].regame();
		info[(int)ePlayer::PLAYER_SECOND].regame();
		ZeroMemory(&scpu, sizeof(scpu));
	}

};
Client c;
int client_cur = 0;

void err_display(const std::string& msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	::cout << "[" << msg << "]" << GetLastError();
	LocalFree(lpMsgBuf);
}

void delay(int ms) {
	clock_t s = clock();
	while (1) {
		clock_t e = clock();
		if (e - s > ms) return;
	}
}

DWORD WINAPI SendThread(LPVOID arg) {

	DWORD retval;
	char buffer = ' ';
	auto threadID = GetCurrentThreadId();
	hClientInfo* client = static_cast<hClientInfo*>(arg);

	cout << "SENDTHREAD" << threadID << endl;
	while (1) {
		WaitForSingleObject(client->hEvent[(int)eTHREAD::THREAD_RECV], INFINITE);

		switch (client->Scene) {
		case eSCENE::SCENE_LOBBY: {
			switch (client->lastpacket) {
			case cs_login: {
				sc_packet_login scpl;
				scpl.id = client->id;
				scpl.size = sizeof(scpl);
				scpl.type = sc_login;
				scpl.f_login = client_table[(int)ePlayer::PLAYER_FIRST];
				scpl.s_login = client_table[(int)ePlayer::PLAYER_SECOND];
				retval = c.sendAll(reinterpret_cast<char*>(&scpl), sizeof(scpl), 0);
				cout << "Send: login " << client->id << endl;
				break;
			}
			case cs_ready: {
				sc_packet_ready scpr;
				scpr.size = sizeof(scpr);
				scpr.type = sc_ready;
				if (c.getReadyAll()) {
					scpr.id = (int)ePlayer::PLAYER_MAX;
					retval = c.sendAll(reinterpret_cast<char*>(&scpr), scpr.size, 0);
					c.info[(int)ePlayer::PLAYER_FIRST].Scene = eSCENE::SCENE_GAME;
					c.info[(int)ePlayer::PLAYER_SECOND].Scene = eSCENE::SCENE_GAME;
					cout << "Send: allready" << endl;
				}
				else {
					scpr.id = client->id;
					retval = c.sendAll(reinterpret_cast<char*>(&scpr), scpr.size, 0);
					cout << "Send: ready " << client->id << endl;
				}
				break;
			}
			}
			break;
		}
		case eSCENE::SCENE_GAME: {
			switch (client->lastpacket) {
			case cs_user: {
				//delay(100);
				if (c.info[(int)ePlayer::PLAYER_FIRST].getUser == true && c.info[(int)ePlayer::PLAYER_SECOND].getUser == true) {
					c.info[(int)ePlayer::PLAYER_FIRST].getUser = false;
					c.info[(int)ePlayer::PLAYER_SECOND].getUser = false;
					c.scpu.type = sc_user;
					c.scpu.size = sizeof(c.scpu);

					retval = c.sendAll(reinterpret_cast<char*>(&c.scpu), sizeof(c.scpu), 0);
					cout << "Send user: all " << endl;
					ZeroMemory(&c.scpu, sizeof(c.scpu));
				}
				break;
			}
			}
			break;
		}
		}
		SetEvent(client->hEvent[(int)eTHREAD::THREAD_SEND]);
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
		WaitForSingleObject(client->hEvent[(int)eTHREAD::THREAD_SEND], INFINITE);

		ZeroMemory(&buffer, sizeof(buffer));
		retval = recv(client->s, buffer, MAXBUFFER, 0);
		if (retval == SOCKET_ERROR)  err_display("recv()");
		else if (retval == 0) return 0;

		//cout << "RECV: " << client->id << " " << (int)client->Scene << " " << buffer[1] << endl;
		switch (client->Scene) {
		case eSCENE::SCENE_LOBBY: {
			switch (buffer[1]) {
			case cs_login: {
				client->lastpacket = cs_login;
				cout << "RECV: cs_login, ID: " << client->id << endl;
				break;
			}
			case cs_ready: {
				client->lastpacket = cs_ready;
				client->isReady = true;
				cout << "RECV: cs_ready, ID: " << client->id << endl;
				break;
			}
			}
			break;
		}
		case eSCENE::SCENE_GAME: {
			switch (buffer[1]) {
			case cs_user: {
				cs_packet_user cspu;
				memcpy(&cspu, buffer, sizeof(cspu));
				memcpy(&client->table, reinterpret_cast<char*>(&cspu.table), sizeof(cspu.table));
				if (cspu.gameEnd == true) {
					client->lastpacket = cs_login;
					client->Scene = eSCENE::SCENE_LOBBY;
					ZeroMemory(&c.scpu, sizeof(c.scpu));
					c.regame();
					break;
				}
				client->pos[(int)ePosition::POS_X] = cspu.pos[(int)ePosition::POS_X];
				client->pos[(int)ePosition::POS_Y] = cspu.pos[(int)ePosition::POS_Y];
				client->skillGauge = cspu.skillGauge;
				client->skillActive = cspu.skillActive;
				client->nextBlock = cspu.nextBlock;
				client->lastpacket = cs_user;
				client->state = static_cast<eHeroState>(cspu.state);
				client->getUser = true;

				memcpy(&c.scpu.table[client->id], reinterpret_cast<char*>(&client->table), sizeof(client->table));
				memcpy(&c.scpu.pos[client->id], client->pos, sizeof(client->pos));
				c.scpu.skillGauge[client->id] = client->skillGauge;
				c.scpu.skillActive[client->id] = client->skillActive;
				c.scpu.nextBlock[client->id] = client->nextBlock;
				c.scpu.state[client->id] = static_cast<BYTE>(client->state);
				//~packet save for server
				cout << "RECV: cs_user, ID: " << client->id << endl;
				break;
			}
			}
			break;
		}
		}
		SetEvent(client->hEvent[(int)eTHREAD::THREAD_RECV]);
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

		c.info[client_cur].addrlen = sizeof(c.info[client_cur].addr);
		c.info[client_cur].s = accept(sock_listen, (sockaddr*)&c.info[client_cur].addr, &c.info[client_cur].addrlen);

		c.info[client_cur].id = client_cur;
		client_table[client_cur] = 1;
		if (c.info[client_cur].s == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}
		c.info[client_cur].hEvent[(int)eTHREAD::THREAD_RECV] = CreateEvent(NULL, false, false, NULL);
		c.info[client_cur].hEvent[(int)eTHREAD::THREAD_SEND] = CreateEvent(NULL, false, true, NULL);

		cout << inet_ntoa(c.info[client_cur].addr.sin_addr) << " " << ntohs(c.info[client_cur].addr.sin_port) << endl;

		hThread = CreateThread(NULL, 0, SendThread, &c.info[client_cur], 0, &c.info[client_cur].dwThreadID[(int)eTHREAD::THREAD_SEND]);
		if (hThread == NULL) { closesocket(c.info[client_cur].s); }
		else {
			SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);
			CloseHandle(hThread);
		}
		hThread = CreateThread(NULL, 0, RecvThread, &c.info[client_cur], 0, &c.info[client_cur].dwThreadID[(int)eTHREAD::THREAD_RECV]);
		if (hThread == NULL) { closesocket(c.info[client_cur].s); }
		else {
			SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);
			CloseHandle(hThread);
		}

	}

	closesocket(sock_listen);
	WSACleanup();
	return 0;
}