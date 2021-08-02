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
	cout << "현재 상태 : bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR) {
		showErrorMessage("listen()");
	}
	cout << "현재 상태 : listen()\n";

	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	cout << "현재 상태 : accept()\n";

	if (clientSocket == INVALID_SOCKET) {
		showErrorMessage("accept()");
	}

	while (true) {
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL;
		cout << "클라이언트 메시지 : " << received << '\n';
		cout << "메시지 전송 : " << received << '\n';

		if (strcmp(received, "[exit]") == 0) {
			send(clientSocket, received, sizeof(received) - 1, 0);
			cout << "서버 종료\n";
			break;
		}

		send(clientSocket, received, sizeof(received) - 1, 0);
	}

	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();

	return 0;
}