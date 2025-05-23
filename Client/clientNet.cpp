#include "clientNet.hpp"
#define PORT 8080

int initClient(){

    int sock;
    struct sockaddr_in servAddr;

    // create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        printf("ERROR: Client socket creation failed\n");
        exit(1);
    }
    printf("Client socket creation successful\n");
    bzero(&servAddr, sizeof(servAddr));

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr("10.42.0.1"); // server ip address
    servAddr.sin_port = htons(PORT);

    // connect socket to server
    if(connect(sock, (struct sockaddr*)&servAddr, sizeof(servAddr)) != 0){
        printf("ERROR: Connection with server failed\n");
        exit(1);
    }
    printf("Client connection with server successful\n");

    // return socket
    return sock;
}