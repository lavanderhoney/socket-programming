#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<winsock2.h>
#include <ws2tcpip.h>

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
    
    if (argc < 2)
    {
        fprintf(stderr,"Port number not provided! \n");
        exit(1);
    }
    int sockfd, new_sockfd, portno, n;
    char buffer[255];

    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    
    sockfd = socket(AF_INET, SOCK_STREAM,0);
    if (sockfd<0)
    {
        error("Error opening socket");
    }
    memset(&server_addr, 0, sizeof(server_addr));

    portno = atoi(argv[1]);
    //argv[0] -> filename and argv[1]->port name

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(portno); //hton -> host to network short ?

    int binding_status = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(binding_status < 0){
        error("Binding failed.");
    }
    
    listen(sockfd, 5);
    client_len = sizeof(client_addr);

    new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len );
    if (new_sockfd<0)
    {
        error("Error on Accept");
    }
    
    while (1)
    {
        memset(&buffer, 0, sizeof(buffer));
        n = recv(new_sockfd, buffer, 255,0);
        if (n<0)
        {
            error("Error on reading!");
        }
        printf("Client: %s \n", buffer);
        
        memset(&buffer, 0, sizeof(buffer));
        fgets(buffer, 255, stdin);
        n = send(new_sockfd, buffer, strlen(buffer),0);
        if (n<0)
        {
            error("Error on writing!");
        }

        int i = strncmp("Bye",buffer,3);
        if (i==0)
        {
            break;
        }
    }
    closesocket(new_sockfd);
    closesocket(sockfd);
    WSACleanup();
    
    return 0;
}