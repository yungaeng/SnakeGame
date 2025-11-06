// server.cpp
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdio>
#include "..\..\SnakeGame\SnakeGame-Server\Server\pch.h" 

#pragma comment(lib, "ws2_32")

// 서버 상수
#define SERVERPORT 9000
#define BUF_SIZE 512

// --- 전역 함수 정의 ---

// 클라이언트로 패킷을 전송하는 헬퍼 함수
void SendPacket(SOCKET sock, const PacketHeader* pkt) {
    if (pkt == nullptr) return;

    // 패킷 헤더에 기록된 정확한 크기만큼 전송
    int sentLen = send(sock, (const char*)pkt, pkt->packetSize, 0);
    if (sentLen == SOCKET_ERROR) {
        printf("Send Error: %d\n", WSAGetLastError());
    }
    else {
        printf("[SEND] ID: %d, Size: %d\n", pkt->packetID, sentLen);
    }
}

// 수신된 완전한 패킷을 처리하고 응답을 생성하는 함수
void ProcessClientPacket(SOCKET client_sock, const char* data, int totalSize,
    const struct sockaddr_in& clientaddr, const char* addr)
{
    // 1. 패킷 헤더에서 ID 추출
    const PacketHeader* header = reinterpret_cast<const PacketHeader*>(data);
    PACKET_ID pid = static_cast<PACKET_ID>(header->packetID);

    printf("\n[RECV] IP: %s, Port: %d, ID: %d, Size: %d\n",
        addr, ntohs(clientaddr.sin_port), (int)pid, totalSize);

    // 2. 패킷 ID에 따라 처리 및 응답
    switch (pid)
    {
    case PACKET_ID::CS_LOGIN:
    {
        const CS_LOGIN_PACKET* loginPkt = reinterpret_cast<const CS_LOGIN_PACKET*>(data);

        // 로그인 정보 출력
        printf("  > LOGIN INFO - Color: RGB(%d,%d,%d), ",
            GetRValue(loginPkt->color),
            GetGValue(loginPkt->color),
            GetBValue(loginPkt->color));
        wprintf(L"Name: %ls\n", loginPkt->name);

        // 3. SC_ENTER_PACKET 응답 생성 및 전송
        SC_ENTER_PACKET enterPkt;

        // 임의의 초기 위치와 정보를 설정
        memcpy(enterPkt.name, loginPkt->name, 20);
        enterPkt.color = loginPkt->color;
        enterPkt.x = 350; // 중앙 위치
        enterPkt.y = 350;

        SendPacket(client_sock, &enterPkt);
        break;
    }
    case PACKET_ID::CS_MOVE:
    {
        // CS_MOVE 처리 로직...
        printf("  > MOVE Packet received. (처리 생략)\n");
        break;
    }
    // ... (다른 패킷 처리)
    default:
    {
        printf("  > Unknown Packet ID: %d\n", (int)pid);
        break;
    }
    }
}


int main(int argc, char* argv[])
{
    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup Error\n");
        return 1;
    }

    // 소켓 생성
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) {
        printf("Socket Error: %d\n", WSAGetLastError());
        WSACleanup();
        return 0;
    }

    // bind()
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);

    if (bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR) {
        printf("Bind Error: %d\n", WSAGetLastError());
        closesocket(listen_sock);
        WSACleanup();
        return 0;
    }

    // listen()
    if (listen(listen_sock, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen Error: %d\n", WSAGetLastError());
        closesocket(listen_sock);
        WSACleanup();
        return 0;
    }

    printf("[TCP SERVER] Server listening on port %d...\n", SERVERPORT);

    // 데이터 통신에 사용할 변수
    SOCKET client_sock;
    struct sockaddr_in clientaddr;
    int addrlen;

    // 수신 버퍼 및 상태 변수
    char recv_buffer[BUF_SIZE];
    int received_bytes = 0;

    while (1) {
        // 1. accept() - 블로킹 모드이므로 여기서 새 클라이언트 연결을 기다립니다.
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            // 오류가 발생하면 루프 종료 대신 로그를 남기고 다음 accept를 시도할 수 있습니다.
            printf("Accept Error: %d\n", WSAGetLastError());
            continue;
        }

        // 접속한 클라이언트 정보 출력
        char addr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
        printf("\n================================================\n");
        printf("[TCP SERVER] Client connected: IP=%s, Port=%d\n", addr, ntohs(clientaddr.sin_port));
        printf("================================================\n");

        received_bytes = 0; // 새 클라이언트 연결 시 버퍼 상태 초기화

        // 2. 클라이언트와 데이터 통신 (블로킹 recv 루프)
        while (1) {
            // 수신 버퍼의 남은 공간에 데이터를 받습니다.
            int recvLen = recv(client_sock, recv_buffer + received_bytes, BUF_SIZE - received_bytes, 0);

            if (recvLen == 0) {
                // 클라이언트 연결 종료
                break;
            }
            else if (recvLen == SOCKET_ERROR) {
                // 수신 오류
                printf("[TCP SERVER] Recv Error: %d\n", WSAGetLastError());
                break;
            }

            // 3. 수신 성공: 누적 바이트 업데이트
            received_bytes += recvLen;

            // 4. 패킷 조립 및 처리 루프
            while (received_bytes >= sizeof(PacketHeader))
            {
                // 헤더 정보 획득
                const PacketHeader* header = reinterpret_cast<const PacketHeader*>(recv_buffer);
                uint8_t totalSize = header->packetSize;

                // 패킷 크기가 버퍼 크기를 초과하는 경우 (오류 또는 비정상 데이터)
                if (totalSize > BUF_SIZE) {
                    printf("!! Packet size too large (%d). Closing connection.\n", totalSize);
                    received_bytes = 0; // 버퍼 비우고 연결 종료
                    goto disconnect;
                }

                // 전체 패킷이 도착했는지 확인
                if (received_bytes < totalSize) {
                    // 불완전 패킷, 다음 수신을 기다림
                    break;
                }

                // 5. 완전한 패킷 처리
                ProcessClientPacket(client_sock, recv_buffer, totalSize, clientaddr, addr);

                // 6. 처리된 패킷만큼 버퍼 이동 및 잔여 바이트 갱신
                received_bytes -= totalSize;
                if (received_bytes > 0)
                {
                    // 남은 데이터를 버퍼 앞으로 이동
                    memmove(recv_buffer,
                        recv_buffer + totalSize,
                        received_bytes);
                }
            }
        }

    disconnect: // 연결 종료 레이블
        // 7. 소켓 닫기
        closesocket(client_sock);
        printf("[TCP SERVER] Client disconnected: IP=%s, Port=%d\n", addr, ntohs(clientaddr.sin_port));
    }

    // 서버 종료 시
    closesocket(listen_sock);
    WSACleanup();
    return 0;
}