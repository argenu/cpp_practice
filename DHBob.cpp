#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <cstdio> 
#include <cstring> 
#include <winsock2.h> 
#include "rsa.h"

void vect_send(vector <int> A, SOCKET client) {
    char buffer[1024];
    for (int j = 0; j < A.size(); j++) {
        buffer[A.size() - j - 1] = A[j] + 48;
    }
    buffer[A.size()] = '\0';

    send(client, buffer, sizeof(buffer), 0);
}

int main() {
    char buffer[1024];
    vector<int> p, filler, g, b, A, B, K;
    WSADATA WSAData;
    SOCKET server, client;
    SOCKADDR_IN serverAddr, clientAddr;
    WSAStartup(MAKEWORD(2, 0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5555);

    bind(server, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    listen(server, 0);
    cout << "Listening for incoming connections...." << endl; 

    int clientAddrSize = sizeof(clientAddr);
    client = accept(server, (SOCKADDR*)&clientAddr, &clientAddrSize);
    cout << "Client connected!" << endl;

    recv(client, buffer, sizeof(buffer), 0);
    cout << "Received p" << endl;
    cout << "p: ";
    vect_set(p, buffer);
    vect_repr(p, "");

    recv(client, buffer, sizeof(buffer), 0);
    cout << "Received g" << endl;
    cout << "g: ";
    vect_set(g, buffer);
    vect_repr(g, "");

    vect_set(b, "1546");
    cout << "b: ";
    vect_repr(b, "");

    gud_pow(g, b, p, B);
    cout << "B: ";
    vect_repr(B, "");

    vect_send(B, client);
    cout << "Sent B" << endl;

    recv(client, buffer, sizeof(buffer), 0);
    cout << "Received A" << endl;

    vect_set(A, buffer);
    cout << "A: ";
    vect_repr(A, "");

    gud_pow(A, b, p, K);
    cout << "Generated key" << endl;
    cout << "K: ";
    vect_repr(K, "");

    closesocket(client);
    WSACleanup();
}