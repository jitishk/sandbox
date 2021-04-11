// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdlib.h>
#define PORT 8080 


void
send_packet(int sock, struct sockaddr_in *serv_addr, in_port_t port)
{
    char *hello = "Hello from client"; 
    serv_addr->sin_port = htons(port); 
    if (connect(sock, (struct sockaddr *)serv_addr, sizeof(*serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return;
    } 
    send(sock, hello, strlen(hello) , 0 ); 
}

int main(int argc, char const *argv[]) 
{ 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char *hello = "Hello from client"; 
    char buffer[1024] = {0}; 
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 

    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(atoi(argv[2])); 
    
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0) 
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
    for (int i=4000; i<9000; i++) {
        send_packet(sock, &serv_addr, i);
    }
    return 0;

//   if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
//   { 
//       printf("\nConnection Failed \n"); 
//       return -1; 
//   } 
//   send(sock , hello , strlen(hello) , 0 ); 
//   printf("Hello message sent\n"); 
//   valread = read( sock , buffer, 1024); 
//   printf("%s\n",buffer ); 
//   return 0; 
} 
