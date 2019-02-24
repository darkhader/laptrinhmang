// TCPClient_Bai1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include "winsock2.h"
#include "ws2tcpip.h"
#include "sstream"
int main(int argc,char *argv[])
{
	WSADATA wsa;
	WORD ver = MAKEWORD(2, 2);
	if (WSAStartup(ver, &wsa) == 0) {
		printf("Khoi tao thanh cong \n");
	}
	else {
		printf("That bai khi khoi tao");
	}
	if (argc > 2) {
		std::istringstream iss(argv[2]);
		int port;
		//convert to int ss
		if (iss >> port)
		{
			// khoi tao
			SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			// khai bao addr voi port dc nhap
			SOCKADDR_IN addr;
			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);
			//check nhap ip hay domain
			if (inet_addr(argv[1]) == -1) {
				printf("nhap vao domain");
				if (strcmp("localhost", argv[1]) == 0) {
					printf("nhap vap localhost");
					addr.sin_addr.s_addr = inet_addr("127.0.0.1");
				}
				else {
					addrinfo * result;
					int res = getaddrinfo(argv[1], "http", NULL, &result);
					memcpy(&addr, result->ai_addr, result->ai_addrlen);
				}			
			}
			else {
				printf("nhap vao ip");
				addr.sin_addr.s_addr = inet_addr(argv[1]);
			}
			//connect
			int ret = connect(client, (SOCKADDR *)&addr, sizeof(addr));
			if (ret == 0) {
				char buf[256];
				while (1) {
					printf("nhap du lieu de gui den server: ");
					gets_s(buf, sizeof(buf));
					send(client, buf, strlen(buf), 0);
					if (strncmp(buf, "exit", 4) == 0) {
						break;
					}
				}
			}
			else {
				printf("ko ket noi dc");
			}
			closesocket(client);
			system("PAUSE");
		}	
	}
	else {
		printf("nhap du tham so,lam on");
	}
	WSACleanup();
	return 0;
}



