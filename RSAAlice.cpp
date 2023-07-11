#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <cstdio> 
#include <cstring> 
#include <winsock2.h>
#include "rsa.h"

int main() {
    int leng, t;
    char buffer[1024], msg[1024];
    string stemp, dec;
    vector<int> p, q, pp, qp, n, e, d, phin, a, res;
    vector<vector<int>> enc;
    WSADATA WSAData;
    SOCKET server;
    SOCKADDR_IN addr;
    WSAStartup(MAKEWORD(2, 0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);
 
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5555);

    connect(server, (SOCKADDR*)&addr, sizeof(addr)); 
    cout << "Connected to server!\nEnter the message: ";

    fgets(msg, 1024, stdin);
    
    get_input(p, q);
    generate_RSA_values(p, q, pp, qp, n, phin, e, d);
    enc_msg(msg, e, n, enc);

    leng = enc.size() - 1;

    itoa(leng, buffer, 10);
    send(server, buffer, sizeof(buffer), 0);

    for (int i = 0; i < leng; i++) {
        for (int j = 0; j < enc[i].size(); j++) {
            buffer[enc[i].size() - j - 1] = enc[i][j] + 48;
        }
        buffer[enc[i].size()] = '\0';
        send(server, buffer, sizeof(buffer), 0);
    }

    cout << "Sent data" << endl;

    closesocket(server);
    WSACleanup();
}