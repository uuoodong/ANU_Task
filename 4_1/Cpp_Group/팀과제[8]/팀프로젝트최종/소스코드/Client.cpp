#include "stdio.h"
#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")

#define DESIRED_WINSOCK_VERSION        0x0202
#define MINIMUM_WINSOCK_VERSION        0x0001

#define MAXBUF 65536

int main()
{
	SOCKET m_client_socket;
	WSADATA wsadata;
	struct sockaddr_in server_addr;
	char buffer[MAXBUF];
	char sendbuffer[256];
	
	// 소켓을 초기화
	if (!WSAStartup(DESIRED_WINSOCK_VERSION, &wsadata)) {
		if (wsadata.wVersion < MINIMUM_WINSOCK_VERSION) {
			WSACleanup();
			exit(1);
		}
	}

	// 소켓을 생성
	m_client_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (m_client_socket == INVALID_SOCKET) {
		printf("socket error : ");
		WSACleanup();
		exit(1);
	}

	// 소켓이 접속할 주소 지정
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(2100);

	// 지정한 주소로 접속
	if (connect(m_client_socket, (LPSOCKADDR)&server_addr, sizeof(server_addr)) != 0) {
		printf("connect error : ");
		exit(1);
	}
	printf("값 입력 : ");
	scanf("%s", sendbuffer);
	send(m_client_socket, sendbuffer, 256, 0);
	// 데이터 처리
	recv(m_client_socket, buffer, MAXBUF, 0);

	// 소켓을 닫음
	closesocket(m_client_socket);

	// 받아온 문자열을 화면에 출력
	printf("\n Recv : %s", buffer);
	return 0;
}
