#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <cstdio> 
#include <cstring> 
#include <winsock2.h> 
#include "rsa.h"

int main() {
    int leng;
    string dec;
    char buffer[1024] = { 0 };
    vector<int> p, q, pp, qp, n, e, d, phin, a, res;
    vector <vector<int>> enc;
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
    leng = atoi(buffer);
    cout << leng << endl;
    enc.resize(leng);
    for (int i = 0; i < leng; i++) {
        recv(client, buffer, sizeof(buffer), 0);
        vect_set(enc[i], buffer);
    }
    get_input(p, q);
    generate_RSA_values(p, q, pp, qp, n, phin, e, d);
    dec_msg(enc, d, n, dec);
    cout << "Decrypted: " << dec << endl;

    closesocket(client);
    WSACleanup();
}