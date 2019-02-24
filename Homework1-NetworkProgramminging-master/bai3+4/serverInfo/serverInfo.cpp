// serverInfo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include "winsock2.h"
#include "sstream"
struct information {
	char name[128];
	char diskNameCharacter[512];
	int size[10];
};

int main(int argc,char *argv[])
{
	std::istringstream iss(argv[1]);
	int port;
	iss>>port;
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);

	bind(listener, (SOCKADDR *)&addr, sizeof(addr));
	listen(listener, 5);
	printf("Dang cho: \n");
	SOCKET client = accept(listener, NULL, NULL);
	printf("Da ket noi : \n");
	char buf[1024];
	int ret = recv(client, buf, sizeof(buf), 0);

	buf[ret] = 0;
	printf("Received: %s\n", buf);

	struct information Info;
	memcpy(&Info, buf, sizeof(Info));
	printf("ten may tinh la: %s \n", Info.name);
	for (int i = 0; i <  sizeof(Info.diskNameCharacter) / sizeof(char); i++) {
		if(Info.diskNameCharacter[i] >= 'A' && Info.diskNameCharacter[i] <= 'Z')
		printf("o cung %c co dung luong %d Mb \n", Info.diskNameCharacter[i],Info.size[i]);
	}
	
	closesocket(client);
	closesocket(listener);

	system("PAUSE");

	WSACleanup();

	return 0;
}
