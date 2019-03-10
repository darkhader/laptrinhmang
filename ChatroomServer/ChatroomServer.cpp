#include "pch.h"
#include <iostream>
#include <WinSock2.h>
#include <ctime>
using namespace std;
DWORD WINAPI ClientThread(LPVOID lpParam);
void Disconnect(SOCKET s);

SOCKET listClients[64];
int no_Client = 0;

int main(int argc, char * argv[]) {
	WSADATA wsa;
	int ret = WSAStartup(0x0202, &wsa);
	if (ret == 0) {
		cout << "Initialize WSA" << endl;
	}
	else {
		cout << "Error while start up ";
		return -2;
	}
	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	int port = atoi(argv[1]);
	if (port < 0) {
		cout << "Wrong port!!";
		return -1;
	}
	SOCKADDR_IN addrin;
	addrin.sin_family = AF_INET;
	addrin.sin_addr.s_addr = htonl(INADDR_ANY);
	addrin.sin_port = htons(port);

	bind(server, (SOCKADDR*)&addrin, sizeof(addrin));
	listen(server, 5);
	cout << "Listen at port " << port << endl;
	while (true) {
		SOCKET client = accept(server, NULL, NULL);
		cout << "Accept new client: " << client << endl;
		listClients[no_Client] = client;
		no_Client++;
		CreateThread(0, 0, ClientThread, &client, 0, 0);
	}
	WSACleanup();
}

DWORD WINAPI ClientThread(LPVOID lpParam) {
	SOCKET client = *(SOCKET*)lpParam;
	char buf[256];
	int ret;
	char id[64] = "";
	char name[64];
	while (true) {
		ret = recv(client, buf, sizeof(buf), 0);
		if (ret <= 0) {
			Disconnect(client);
			break;
		}
		buf[ret] = 0;
		if (strcmp(id, "client_id:")) {
			sscanf(buf, "%s%s", id, name);
		}
		else {
			cout << "Receive: " << buf << " from " << buf << endl;
			for (int i = 0; i < no_Client; i++)
			{
				if (listClients[i] == client) continue;
				// get system current time string 
				time_t rawtime;
				struct tm * timeinfo;
				char timestring[80];
				time(&rawtime);
				timeinfo = localtime(&rawtime);
				strftime(timestring, sizeof(timestring), "[%d-%m-%Y %H:%M:%S] ", timeinfo);
				// 
				send(listClients[i], timestring, strlen(timestring), 0);
				send(listClients[i], name, strlen(name), 0);
				send(listClients[i], ": ", 2, 0);
				send(listClients[i], buf, strlen(buf), 0);
			}
		}
	}
	return 0;
}

void Disconnect(SOCKET s) {
	cout << s << " disconnected" << endl;
	int i = 0;
	for (; i < no_Client; i++) {
		if (listClients[i] == s)
			break;
	}
	if (i < no_Client - 1) {
		listClients[i] = listClients[no_Client - 1];
	}
	no_Client--;
}