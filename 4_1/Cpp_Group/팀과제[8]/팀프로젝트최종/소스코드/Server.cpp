#include "stdio.h"
#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")

#define DESIRED_WINSOCK_VERSION        0x0202
#define MINIMUM_WINSOCK_VERSION        0x0001

#define MAXBUF 65536

int main()
{
	SOCKET m_client_sock, m_listen_sock; // 소켓 디스크립트 정의
	WSADATA wsadata;
	struct sockaddr_in client_addr, server_addr;

	// 클라이언트에 보내줄 문자열
	char buffer[4];  
	char buffer2[MAXBUF] = {0,};
	char recvbuffer[256];
	int clen;
	int i = 0;
	memset(buffer, 0, 4);
	memset(recvbuffer, 0, 256);
	// 소켓을 초기화 
	if (!WSAStartup(DESIRED_WINSOCK_VERSION, &wsadata)) {
		if (wsadata.wVersion < MINIMUM_WINSOCK_VERSION) {
			WSACleanup();
			exit(1);
		}
	}

	// 서버 소켓 생성
	m_listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_listen_sock == INVALID_SOCKET) {
		printf("socket error : ");
		WSACleanup();
		exit(1);
	}

	clen = sizeof(server_addr);

	// 주소 구조체에 주소 지정
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(2100); // 사용할 포트번호

	// bind 함수를 사용하여 서버 소켓의 주소 설정
	if (bind(m_listen_sock, (LPSOCKADDR)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
		printf("bind error : ");
		WSACleanup();
		exit(1);
	}

	// 위에서 지정한 주소로 클라이언트 접속을 기다림
	if (listen(m_listen_sock, 1) != 0) {
		printf("listen error : ");
		exit(1);
	}

	while (1) {
		int number = 10000;
		int heap[10000];
		m_client_sock = accept(m_listen_sock, (LPSOCKADDR)&client_addr, &clen);
		if (m_client_sock != INVALID_SOCKET) {
			printf("=== 수신 대기중 ===\n");
			recv(m_client_sock, recvbuffer, 256, 0);
			printf("=== 계산 중 ===\n");
			int selectFlag = 0;
			for (int i = number; i > 0; i--) {
				heap[selectFlag] = i;
				selectFlag++;
			}
			for (int i = 1; i < number; i++) {
				int c = i;
				do {
					int root = (c - 1) / 2;
					if (heap[root] < heap[c]) {
						int temp = heap[root];
						heap[root] = heap[c];
						heap[c] = temp;
					}
					c = root;
				} while (c != 0);
			}
			// 크기를 줄여가며 반복적으로 힙을 구성
			for (int i = number - 1; i >= 0; i--) {
				int temp = heap[0];
				heap[0] = heap[i];
				heap[i] = temp;
				int root = 0;
				int c = 1;
				do {
					c = 2 * root + 1;
					// 자식 중에 더 큰 값을 찾기 
					if (c < i - 1 && heap[c] < heap[c + 1]) {
						c++;
					}
					// 루트보다 자식이 크다면 교환 
					if (c < i && heap[root] < heap[c]) {
						temp = heap[root];
						heap[root] = heap[c];
						heap[c] = temp;
					}
					root = c;
				} while (c < i);
			}
			// 결과 출력 
			for (int i = 0; i < number; i++) {
				printf("%d ", heap[i]);
				sprintf(buffer, "%d", heap[i]);
				strcat(buffer2, buffer);
				strcat(buffer2, " ");
			}
			
			printf("=== 계산 완료 ===\n");
			printf("%s", buffer2);
			if (send(m_client_sock, buffer2, MAXBUF, 0) <= 0) printf("send error : ");
			printf("=== 전송 완료 ===\n");
				
			closesocket(m_client_sock);
		}
	}
	return 0;
}
