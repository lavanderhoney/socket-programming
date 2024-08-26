/*filename server_ipaddress protno */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<winsock2.h>
#include <ws2tcpip.h>
// #include<netdb.h>

#pragma comment(lib, "ws2_32.lib") 

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        return 1;
    }
    
    if (argc < 3)
    {
        fprintf(stderr,"usage %s hostname port \n",argv[0]);
        exit(1);
    }
    int sockfd, portno, n;
    char buffer[255];
    portno = atoi(argv[2]);

    struct sockaddr_in server_addr;
    struct hostent *server;
    sockfd = socket(AF_INET, SOCK_STREAM,0);
    if (sockfd<0)
    {
        error("Error opening socket");
    }
    server = gethostbyname(argv[1]);
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    memcpy((char *) &server_addr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
    server_addr.sin_port = htons(portno);

    int connect_status = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (connect_status<0)
    {
        error("Connect failed");
    }
    
    while(1){
        memset(&buffer, 0, sizeof(buffer));
        fgets(buffer,255,stdin);
        n = send(sockfd, buffer, 255,0);
        if(n<0)
            error("Error on writing");
        
        int i = strncmp("Bye",buffer,3);
        if (i==0)
        {
            break;
        }
        memset(&buffer,0,sizeof(buffer));
        n = recv(sockfd, buffer, 255,0);
        if(n<0)
            error("Error on reading");
        printf("Server: %s \n",buffer);
    }

    closesocket(sockfd);
    WSACleanup();
    return 0;
}