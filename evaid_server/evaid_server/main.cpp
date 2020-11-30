#include "stdafx.h"
#include "protocol.h"

using std::cout;
using std::endl;

enum class eTHREAD { THREAD_SEND = 0, THREAD_RECV = 1, THREAD_MAX = 2 };
enum class eSCENE { SCENE_LOBBY = 0, SCENE_GAME = 1, SCENE_DUMMY = 2, SCENE_MAX = 3 };
enum class ePlayer { PLAYER_FIRST = 0, PLAYER_SECOND = 1, PLAYER_MAX = 2 };
enum class ePosition { POS_X = 0, POS_Y = 1, POS_MAX = 2 };
enum class eBlock { BLOCK_A = 0, BLOCK_B = 1, BLOCK_C = 2, BLOCK_D = 3, BLOCK_E = 4, BLOCK_NONE = 5, BLOCK_STACK = 6, BLOCK_SHADOW = 7 };
constexpr BYTE TABLE_WIDTH = 10;
constexpr BYTE TABLE_HEIGHT = 20;

sc_packet_user g_scpu;

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
	bool getUser;
public:
	hClientInfo() {
		id = NULLVAL;
		isReady = false;
		Scene = eSCENE::SCENE_LOBBY;
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
hClientInfo cInfo[CLIENT_LIMITE];
int client_cur = 0;

void err_display(const std::string& msg) {
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

bool getReadyAll() {
	if (cInfo[0].isReady == true && cInfo[1].isReady == true) {
		cInfo[0].isReady = false;
		cInfo[1].isReady = false;
		return true;
	}
	else return false;
}

int sendall(char* buffer, int bufferSize, int flags) {
	int cnt = 0;
	for (int i = 0; i < CLIENT_LIMITE; ++i) {
		if (cInfo[i].id != NULLVAL) {
			++cnt;
			int retval = send(cInfo[i].s, buffer, bufferSize, flags);
		}
	}
	return cnt;
}

void delay(int ms) {
	clock_t s = clock();
	while (1) {
		clock_t e = clock();
		if (e - s > ms) return;
	}
}

void showtable() {
	
	for (int i = 0; i < TABLE_HEIGHT; ++i) {
		for (int j = 0; j < TABLE_WIDTH; ++j) {
			cout << (int)g_scpu.table[(int)ePlayer::PLAYER_FIRST].t[j][i];
		}
		cout << endl;
	}
	cout << "---------------------" << endl;

	for (int i = 0; i < TABLE_HEIGHT; ++i) {
		for (int j = 0; j < TABLE_WIDTH; ++j) {
			cout << (int)g_scpu.table[(int)ePlayer::PLAYER_SECOND].t[j][i];
		}
		cout << endl;
	}
}

static int GetPosInValX(int x) { return ((x - 100/*PTStartX*/) / 40/*BLOCK_SIZE*/); }
static int GetPosInValY(int y) { return ((y - 50/*PTStartY*/) / 40/*BLOCK_SIZE*/); }

int GameOverCheck() {
	for (int i = 0; i < (int)ePlayer::PLAYER_MAX; ++i) {
		int& centerX = g_scpu.pos[i][(int)ePosition::POS_X];
		int& centerY = g_scpu.pos[i][(int)ePosition::POS_Y];
		if ((g_scpu.table[i].t[GetPosInValX(centerX - 17)][GetPosInValY(centerY)] != (BYTE)eBlock::BLOCK_NONE && g_scpu.table[i].t[GetPosInValX(centerX - 17)][GetPosInValY(centerY)] != (BYTE)eBlock::BLOCK_SHADOW)
			|| (g_scpu.table[i].t[GetPosInValX(centerX + 17)][GetPosInValY(centerY)] != (BYTE)eBlock::BLOCK_NONE && g_scpu.table[i].t[GetPosInValX(centerX + 17)][GetPosInValY(centerY)] != (BYTE)eBlock::BLOCK_SHADOW)) {
			return i;
		}
	}
	return (int)ePlayer::PLAYER_MAX;
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
				retval = sendall(reinterpret_cast<char*>(&scpl), sizeof(scpl), 0);
				cout << "Send: login " << client->id << endl;
				break;
			}
			case cs_ready: {
				sc_packet_ready scpr;
				scpr.size = sizeof(scpr);
				scpr.type = sc_ready;
				if (getReadyAll()) {
					scpr.id = (int)ePlayer::PLAYER_MAX;
					retval = sendall(reinterpret_cast<char*>(&scpr), scpr.size, 0);
					cInfo[0].Scene = eSCENE::SCENE_GAME;
					cInfo[1].Scene = eSCENE::SCENE_GAME;
					cout << "Send: allready" << endl;
				}
				else {
					scpr.id = client->id;
					retval = sendall(reinterpret_cast<char*>(&scpr), scpr.size, 0);
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
				if (cInfo[(int)ePlayer::PLAYER_FIRST].getUser == true && cInfo[(int)ePlayer::PLAYER_SECOND].getUser == true) {
					cInfo[(int)ePlayer::PLAYER_FIRST].getUser = false;
					cInfo[(int)ePlayer::PLAYER_SECOND].getUser = false;
					g_scpu.type = sc_user;
					g_scpu.size = sizeof(g_scpu);
					
					g_scpu.loser = GameOverCheck();
					retval = sendall(reinterpret_cast<char*>(&g_scpu), sizeof(g_scpu), 0);
					cout << "Send user: all " << endl;
					ZeroMemory(&g_scpu, sizeof(g_scpu));
				}
				break;
			}
			}
			break;
		}
		case eSCENE::SCENE_DUMMY: {
			switch (client->lastpacket) {
			case cs_user: {

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
				client->pos[(int)ePosition::POS_X] = cspu.pos[(int)ePosition::POS_X];
				client->pos[(int)ePosition::POS_Y] = cspu.pos[(int)ePosition::POS_Y];
				client->skillGauge = cspu.skillGauge;
				client->skillActive = cspu.skillActive;
				client->nextBlock = cspu.nextBlock;
				client->lastpacket = cs_user;
				client->getUser = true;
				//~packet save for cInfo (필요한가 -> 데이터 처리를 위해 필요하다, 지금은 단순히 넘기기만해서 필요없어보임)
				memcpy(&g_scpu.table[client->id], reinterpret_cast<char*>(&client->table), sizeof(client->table));
				memcpy(&g_scpu.pos[client->id], client->pos, sizeof(client->pos));
				g_scpu.skillGauge[client->id] = client->skillGauge;
				g_scpu.skillActive[client->id] = client->skillActive;
				g_scpu.nextBlock[client->id] = client->nextBlock;
				//~packet save for server
				cout << "RECV: cs_user, ID: " << client->id << endl;
				break;
			}
			}
			break;
		}
		case eSCENE::SCENE_DUMMY: {
			switch (buffer[1]) {
			case cs_user: {
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

		cInfo[client_cur].addrlen = sizeof(cInfo[client_cur].addr);
		cInfo[client_cur].s = accept(sock_listen, (sockaddr*)&cInfo[client_cur].addr, &cInfo[client_cur].addrlen);

		cInfo[client_cur].id = client_cur;
		client_table[client_cur] = 1;
		if (cInfo[client_cur].s == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}
		cInfo[client_cur].hEvent[(int)eTHREAD::THREAD_RECV] = CreateEvent(NULL, false, false, NULL);
		cInfo[client_cur].hEvent[(int)eTHREAD::THREAD_SEND] = CreateEvent(NULL, false, true, NULL);

		cout << inet_ntoa(cInfo[client_cur].addr.sin_addr) << " " << ntohs(cInfo[client_cur].addr.sin_port) << endl;

		hThread = CreateThread(NULL, 0, SendThread, &cInfo[client_cur], 0, &cInfo[client_cur].dwThreadID[(int)eTHREAD::THREAD_SEND]);
		if (hThread == NULL) { closesocket(cInfo[client_cur].s); }
		else {
			SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);
			CloseHandle(hThread);
		}
		hThread = CreateThread(NULL, 0, RecvThread, &cInfo[client_cur], 0, &cInfo[client_cur].dwThreadID[(int)eTHREAD::THREAD_RECV]);
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