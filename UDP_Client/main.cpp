#include <iostream>
#include <WS2tcpip.h>
#include <vector>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

int sentResult;

SOCKET in;
string message;
sockaddr_in receiver, sender;
vector<sockaddr_in> addreses;

void setcur(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
};

void RefreshServers() {
	sentResult = sendto(in, message.c_str(), message.size(), 0, (sockaddr*)&receiver, sizeof(receiver));
	cout << "Servers:\n";
	for (auto const& addr : addreses) {
		char ip[256];
		inet_ntop(AF_INET, &addr.sin_addr, ip, 256);
		cout << ip << endl;
	}
	Sleep(1000);
	system("cls");
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


	int senderLength = sizeof(sender);

	receiver.sin_addr.S_un.S_addr = INADDR_BROADCAST;
	receiver.sin_family = AF_INET;
	receiver.sin_port = htons(54000);

	char buf[1024];
	message = "Broadcast work!";
	
	system("cls");
	char choice;

	//HANDLE sendIp = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);
	//HANDLE input = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)InputHandler, NULL, NULL, NULL);

	RefreshServers();
	while (true)
	{
		ZeroMemory(buf, 1024);
		ZeroMemory(&sender, senderLength);
		char choice;
		cin >> choice;
		if (choice == 'r') {
			RefreshServers();
		}

		if (sentResult != 0) {
			recvfrom(in, buf, 1024, 0, (sockaddr*)&sender, &senderLength);
		}
		
		system("cls");
		char ip[256];
		inet_ntop(AF_INET, &sender.sin_addr, ip, 256);
		addreses.push_back(sender);
	}

	// Close socket
	closesocket(in);

	// Shutdown winsock
	// CloseHandle(sendIp);
	WSACleanup();
}
