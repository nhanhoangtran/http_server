// Server side C program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define SERVERPORT 8080
#define BUFFERSIZE 4096

int main(int argc, char const *argv[])
{
    int server_fd, new_socket; long valread;
    struct sockaddr_in server_addr;
    int addrlen = sizeof(server_addr);
    const int opt = 1;
    
    char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVERPORT);
    
    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

    // setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    // Run on SMP8673B: protocol not available
    // check(setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)), "setsockopt server_socket SO_REUSEPORT failed!");
        
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&server_addr, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char buffer[BUFFERSIZE] = {0};
        valread = read(new_socket , buffer, BUFFERSIZE);
        printf("%s\n", buffer );
        write(new_socket , hello , strlen(hello));
        printf("------------------Hello message sent-------------------\n");
        close(new_socket);
    }
    return 0;
}