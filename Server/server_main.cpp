#include <iostream>
#include <string>
#include <WinSock2.h>

using namespace std;

void showErrorMessage(string message) {
	cout << "error : " << message << '\n';
	system("pause");
	exit(1);
}

int main() {
	cout << "version 0.2\n";

	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;

	int serverPort = 9876;
	char received[256];

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		showErrorMessage("WSAStartUp()");
	}

	serverSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (serverSocket == INVALID_SOCKET) {
		showErrorMessage("socket()");
	}

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(serverPort);

	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		showErrorMessage("bind()");
	}
	cout << "���� ���� : bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR) {
		showErrorMessage("listen()");
	}
	cout << "���� ���� : listen()\n";

	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	cout << "���� ���� : accept()\n";

	if (clientSocket == INVALID_SOCKET) {
		showErrorMessage("accept()");
	}

	while (true) {
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL;
		cout << "Ŭ���̾�Ʈ �޽��� : " << received << '\n';
		cout << "�޽��� ���� : " << received << '\n';

		if (strcmp(received, "[exit]") == 0) {
			send(clientSocket, received, sizeof(received) - 1, 0);
			cout << "���� ����\n";
			break;
		}

		send(clientSocket, received, sizeof(received) - 1, 0);
	}

	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();

	return 0;
}