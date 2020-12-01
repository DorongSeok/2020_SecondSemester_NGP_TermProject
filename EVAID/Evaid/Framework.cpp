#include "stdafx.h"
#include "Framework.h"
#include "logoScene.h"
#include "LobbyScene.h"
#include "ResultScene.h"
#include "GameScene.h"

CFramework::CFramework() {
    if (WSAStartup(MAKEWORD(2, 2), &wsa)) cout << "wsa error" << endl;
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) cout << "sock error" << endl;
    DWORD recvTimeout = 1000; //1000ms
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&recvTimeout, sizeof(recvTimeout));
    GameOver_1 = false;
    GameOver_2 = false;
    FirstConnect = true;
    memset(ServerIP, NULL, sizeof(ServerIP));
}

void CFramework::update(long TimerTick) {
    if (m_pCurrentScene) m_pCurrentScene->update(TimerTick);
}

void CFramework::draw(HDC hDC) {
    clear();
    ReleaseBackBuffer();
    CreateBackBuffer();
    if (m_pCurrentScene) {
        m_pCurrentScene->draw(m_hDCBackBuffer);
    }
    DoubleBuffering(hDC);
}

void CFramework::run(long TimerTick) {
    update(TimerTick);
    InvalidateRect(m_hWnd, &m_rcClient, false);
}

bool CFramework::init(HWND hWnd) {
    m_hWnd = hWnd;
    ::GetClientRect(hWnd, &m_rcClient);
    SetTimer(hWnd, timer_Main, FPS, NULL);

    auto pLogo = new CLogoScene{};
    if (!pLogo->init(this, m_hWnd)) {
        delete pLogo;
        return false;
    }
    m_pCurrentScene = pLogo;

    return true;
}

bool CFramework::AddScene(eSCENE scene) {
    switch (scene)
    {
    case eSCENE::SCENE_LOGO:
        {
            auto pLogo = new CLogoScene{};
            if (!pLogo->init(this, m_hWnd)) {
                delete pLogo;
                return false;
            }
            m_pNextScene = pLogo;
            break;
        }
    case eSCENE::SCENE_LOBBY:
        {
            auto pLobby = new CLobbyScene{ FirstConnect };
            if (!pLobby->init(this, m_hWnd)) {
                delete pLobby;
                return false;
            }
            m_pNextScene = pLobby;
            break;
        }
    case eSCENE::SCENE_GAME:
        {
            auto pGame = new CGameScene{};
            if (!pGame->init(this, m_hWnd)) {
                delete pGame;
                return false;
            }
            m_pNextScene = pGame;
            break;
        }
    case eSCENE::SCENE_RESULT:
        {
            auto pResult = new CResultScene{};
            if (!pResult->init(this, m_hWnd)) {
                delete pResult;
                return false;
            }
            m_pNextScene = pResult;
            break;
        }
    }
    return true;
}

void CFramework::PopScene() {
    cout << "popScene" << m_pCurrentScene << endl;

    CScene* pScene = m_pCurrentScene;
    m_pCurrentScene = nullptr;

    m_pCurrentScene = m_pNextScene;

    //delete pScene;
}

void CFramework::ReleaseObject() {
    KillTimer(m_hWnd, timer_Main);
    m_pCurrentScene->ReleaseObjects();
    m_pNextScene->ReleaseObjects();
    ReleaseBackBuffer();
}

bool CFramework::Mouse(UINT msg, WPARAM w, LPARAM l) {
    if (m_pCurrentScene && m_pCurrentScene->Mouse(msg, w, l)) return true;
}

bool CFramework::KeyBoard(UINT msg, WPARAM w, LPARAM l) {
    if (m_pCurrentScene && m_pCurrentScene->Keyboard(msg, w, l))return true;

    switch (msg) {
    case WM_KEYDOWN:
        switch (w) {
        case VK_ESCAPE:
            DestroyWindow(m_hWnd);
            break;
        default:
            return FALSE;
        }
        break;
    default:
        return FALSE;
        break;
    }
    return TRUE;
}

void CFramework::CreateBackBuffer() {
    ReleaseBackBuffer();

    m_hDCBackBuffer = CreateCompatibleDC(nullptr);
    HDC hDC = GetDC(m_hWnd);
    m_hbmpBackBuffer = CreateCompatibleBitmap(hDC, m_rcClient.right, m_rcClient.bottom);
    SelectObject(m_hDCBackBuffer, m_hbmpBackBuffer);

    ReleaseDC(m_hWnd, hDC);
}

void CFramework::ReleaseBackBuffer() {
    SafeRelease(m_hbmpBackBuffer);
    SafeRelease(m_hDCBackBuffer);
}

void CFramework::DoubleBuffering(HDC hDC) {
    ::BitBlt(hDC,
        m_rcClient.left, m_rcClient.top,
        m_rcClient.right - m_rcClient.left,
        m_rcClient.bottom - m_rcClient.top,
        m_hDCBackBuffer, m_rcClient.left, m_rcClient.top,
        SRCCOPY);
}

void CFramework::clear() {
    SetDCBrushColor(m_hDCBackBuffer, m_hColorBackground);
    FillRect(m_hDCBackBuffer, &m_rcClient, static_cast<HBRUSH>(GetStockObject(DC_BRUSH)));
}
//
int CFramework::recvn(SOCKET s, char* buf, int len, int flags)
{
    int received;
    char* ptr = buf;
    int left = len;

    while (left > 0) {
        received = recv(s, ptr, left, flags);
        if (received == SOCKET_ERROR)
            return SOCKET_ERROR;
        else if (received == 0)
            break;
        left -= received;
        ptr += received;
    }

    return (len - left);

}
//
//void CFramework::err_display(const char* msg)
//{
//   LPVOID lpMsgBuf;
//   FormatMessage(
//      FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
//      NULL, WSAGetLastError(),
//      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//      (LPTSTR)&lpMsgBuf, 0, NULL);
//   printf("[%s] %s", msg, (char*)lpMsgBuf);
//   LocalFree(lpMsgBuf);
//}
//