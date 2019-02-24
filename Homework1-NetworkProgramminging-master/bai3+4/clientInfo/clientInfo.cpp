

#include <stdio.h>
#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <string>
#include "sstream"
#include "ws2tcpip.h"
#include "winsock2.h"
#include <windows.h>
#define _CRT_SECURE_NO_WARNINGS
#define INFO_BUFFER_SIZE 32767

struct information {
	char name[128];
	char diskNameCharacter[512];
	int size[10];
};

int main(int argc,char *argv[])
{
	TCHAR  temporName[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;
	DWORD mydrives = 100;// buffer length
	char lpBuffer[100];// buffer for drive string storage
	int c = 0;
	int counterSize = 0;
	__int64 freeBytesToCaller,total, free;
	struct information Info;
	char allName[128];

	//get name of computer
	bufCharCount = INFO_BUFFER_SIZE;
	if (!GetComputerName(temporName, &bufCharCount))
		printf("ko lay dc");
	char nameComputer[512];
	wcstombs(nameComputer, temporName, wcslen(temporName) + 1);
	
	
	strncpy(Info.name, nameComputer,sizeof(nameComputer));
	printf("ten may la: %s \n",Info.name);
	// Copy the hardware information to the SYSTEM_INFO structure. 

	int getDiskname = GetLogicalDriveStrings(mydrives, (LPWSTR) lpBuffer);

	for (int i = 0; i < getDiskname * 2; i++) {	
		if ('A' <= lpBuffer[i] && lpBuffer[i] <= 'Z') {
			allName[c] = lpBuffer[i];
			c++;
		}
	}   

	for (int i = 0; i < c; i++) {
		char Dname[5];
		Dname[0] = allName[i];
		Dname[1] = ':';
		Dname[2] = '\\';
		Dname[3] = '\\';
		Dname[4] = 0;
		wchar_t wtext[20];
		mbstowcs(wtext, Dname, strlen(Dname) + 1);//Plus null
		LPWSTR ptr = wtext;
		bool getDiskStorage = GetDiskFreeSpaceEx(ptr, (PULARGE_INTEGER)&freeBytesToCaller, (PULARGE_INTEGER )&total, (PULARGE_INTEGER)&free);
		if (getDiskStorage) {
			Info.size[counterSize] = (int)(total / 1048576);
			Info.diskNameCharacter[counterSize] = allName[i];
			printf("o dia %c co dung luong %d MB!\n", Info.diskNameCharacter[i], Info.size[counterSize],total);	
			counterSize++;
		}
	}
	Info.diskNameCharacter[counterSize + 1] = 0;
	// khoi tao wsa
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	std::istringstream iss(argv[2]);
	int port;
	SOCKADDR_IN addr;
	//convert to int ss
	if (iss >> port)
	{
		printf("\n connect den %s o port %d", argv[1], port);
		//lay dia chi ip va gan vao
		if (strcmp("localhost", argv[1]) == 0) {
			addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		}
		else {
			addrinfo * result;
			int res = getaddrinfo(argv[1], "http", NULL, &result);
			memcpy(&addr, result->ai_addr, result->ai_addrlen);
		}
		
		// khai bao addr voi port dc nhap
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);

		//connect
		int ret = connect(client, (SOCKADDR *)&addr, sizeof(addr));
		send(client, (char *)&Info, sizeof(Info), 0);
		
	}
	closesocket(client);
	WSACleanup();
	system("PAUSE");
	return 0;
}

