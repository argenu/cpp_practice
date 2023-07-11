#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <cstdio> 
#include <cstring> 
#include <winsock2.h>
#include "rsa.h"

void vect_send(vector <int> A, SOCKET server) {
    char buffer[1024];
    for (int j = 0; j < A.size(); j++) {
        buffer[A.size() - j - 1] = A[j] + 48;
    }
    buffer[A.size()] = '\0';

    send(server, buffer, sizeof(buffer), 0);
}

int main() {
    char buffer[1024];
    string stemp, dec;
    vector <int> p, g, a, filler, A, B, K;
    WSADATA WSAData;
    SOCKET server;
    SOCKADDR_IN addr;
    WSAStartup(MAKEWORD(2, 0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5555);

    connect(server, (SOCKADDR*)&addr, sizeof(addr)); 
    cout << "Connected" << endl;
    get_input(p, filler);
    prime_root(p, g);
    cout << "p: ";
    vect_repr(p, "");
    cout << "g: ";
    vect_repr(g, "");

    vect_set(a, "17304");

    cout << "a: ";
    vect_repr(a, "");

    gud_pow(g, a, p, A);

    cout << "A: ";
    vect_repr(A, "");

    vect_send(p, server);
    vect_send(g, server);
    vect_send(A, server);
    cout << "Sent p, g, A" << endl;

    recv(server, buffer, sizeof(buffer), 0);

    cout << "Received B" << endl;
    vect_set(B, buffer);
    cout << "B: ";
    vect_repr(B, "");

    gud_pow(B, a, p, K);
    cout << "Generated K" << endl;
    cout << "K: ";
    vect_repr(K, "");

    closesocket(server);
    WSACleanup();
}