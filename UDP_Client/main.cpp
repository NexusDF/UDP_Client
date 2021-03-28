#include <iostream>
#include <WS2tcpip.h>
#include <vector>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

int result;

SOCKET in;
string message;
sockaddr_in receiver, sender;
vector<sockaddr_in> addreses;


void DrawList() {
	cout << "Servers:\n";
	for (auto const& addr : addreses) {
		char ip[256];
		inet_ntop(AF_INET, &addr.sin_addr, ip, 256);
		cout << ip << endl;
	}
}

void ClientHandler() {
	char buf[1024];
	message = "Broadcast work!";
	int senderLength = sizeof(sender);

	while (true) {
		ZeroMemory(buf, 1024);
		ZeroMemory(&sender, senderLength);
		int recvRes = recvfrom(in, buf, 1024, 0, (sockaddr*)&sender, &senderLength);
		if (recvRes != SOCKET_ERROR) {
			char ip[256];
			system("cls");
			inet_ntop(AF_INET, &sender.sin_addr, ip, 256);
			addreses.push_back(sender);
		}
	}
}



void RefreshServers() {
	sendto(in, message.c_str(), message.size(), 0, (sockaddr*)&receiver, sizeof(receiver));
	addreses.clear();
	system("cls");
	DrawList();
}

void main()
{
	setlocale(LC_CTYPE, "rus");

	WSADATA data;
	WORD version = MAKEWORD(2, 2);

	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0)
	{
		cout << "Can't start Winsock! " << wsOk;
		return;
	}

	in = socket(AF_INET, SOCK_DGRAM, 0);

	char broadcast = '1';
	int broadOk = setsockopt(in, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
	if (broadOk != 0)
	{
		cout << "Error in setting Broadcast option";
		closesocket(in);
		return;
	}

	receiver.sin_addr.S_un.S_addr = INADDR_BROADCAST;
	receiver.sin_family = AF_INET;
	receiver.sin_port = htons(54000);

	system("cls");
	char choice;

	HANDLE sendIp = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);
	//HANDLE input = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)InputHandler, NULL, NULL, NULL);

	//RefreshServers();
	cout << "Для обновления введите (r)\n";

	while (true)
	{
		char choice;
		cin >> choice;
		if (choice == 'r') {
			RefreshServers();
		}
	}

	// Close socket
	closesocket(in);

	// Shutdown winsock
	CloseHandle(sendIp);
	WSACleanup();
}
