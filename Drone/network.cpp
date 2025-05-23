#include "network.hpp"
#define PORT 8080

int initServer(){
    int sock, con;
    socklen_t len;
    struct sockaddr_in servAddr, cliAddr;

    // create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        printf("ERROR: Server socket creation failed\n");
        exit(1);
    }
    printf("Server socket creation successful\n");
    bzero(&servAddr, sizeof(servAddr));

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(PORT);

    // bind socket and server
    if((bind(sock, (struct sockaddr*)&servAddr, sizeof(servAddr))) != 0){
        printf("ERROR: Server socket bind failed\n");
        exit(1);
    }
    printf("Server socket bind successful\n");

    // listen on socket
    if((listen(sock, 5)) != 0){
        printf("ERROR: Server listen failed\n");
        exit(1);
    }
    printf("Server listen successful\n");
    len = sizeof(cliAddr);

    // accept connection from client
    con = accept(sock, (struct sockaddr*)&cliAddr, &len);
    if(con < 0){
        printf("ERROR: Server accept failed\n");
        exit(1);
    }
    printf("Server accept successful\n");

    // return connection
    return con;
}