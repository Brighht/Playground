#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#define ISVALIDSOCKET(s) ((s) > 0)
#define GETSOCKETERRNO() (errno)
#define CLOSESOCKET(s) (close(s))


int main(){

    /* Describe the required type of connection you want to make */
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints)); // Initialize the 'hints' structure to zero to set default values
    hints.ai_family = AF_INET;       // Specify IPv4 (use AF_INET6 for IPv6, or AF_UNSPEC for both)
    hints.ai_flags = AI_PASSIVE;     // Indicate that the socket is intended for binding (used with server-side sockets)
    hints.ai_socktype = SOCK_STREAM; // Specify the socket type as TCP (SOCK_STREAM is used for reliable, connection-oriented streams)

    /*Describe the res struct which returns a pointer to pointers of bindable addresses*/
    struct addrinfo *bind_address;
    const char *host = 0;
    const char *service = "8080";
    getaddrinfo(host,service,&hints, &bind_address);

    //create a socket
    printf("Creating a socket...\n");
    int socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
    if(!ISVALIDSOCKET(socket_listen)){
        fprintf(stderr, "socket() failed [%s]", strerror(GETSOCKETERRNO()));
        return 1;
    }

    //bind address to local address
    printf("Binding socket to local address\n");
    //bind() returns 0 on success so if == 1 then it failed
    if (bind(socket_listen,bind_address->ai_addr, bind_address->ai_addrlen)){   
        fprintf(stderr, "bind() failed [%s]", strerror(GETSOCKETERRNO()));
        return 1;
    }

    //free bind_address
    freeaddrinfo(bind_address);

    //ready to listen to request
    printf("Server is Listening on port %s...\n", service);
    if(listen(socket_listen, 10) < 0){
        fprintf(stderr, "listen() failed [%s]", strerror(GETSOCKETERRNO()));
        return 1;
    }

    //ready for some connections
    printf("waiting for client to make a request...\n");
    struct sockaddr_storage client_address;     //store addres of client
    socklen_t client_len = sizeof(client_address);
    int socket_client = accept(socket_listen,(struct sockaddr*)&client_address,&client_len);
    if(!ISVALIDSOCKET(socket_client)){
        fprintf(stderr, "accept() failed [%s]", strerror(GETSOCKETERRNO()));
        return 1;
    }

    //connection has been secured 
    printf("Client is connected...");
    char addr_buffer[100];
    getnameinfo((struct sockaddr*)&client_address,client_len,addr_buffer, sizeof(addr_buffer), 0 , 0, NI_NUMERICHOST);
    printf("(%s)\n", addr_buffer);

    //receiving request 
    printf("Reading request...\n");
    char request[1024];
    int bytes_received = recv(socket_client,request,1024,0);
    printf("Received %d bytes.\n",bytes_received);
    printf("%.*s",bytes_received, request);

    //response
    printf("Sending response...\n"); 
    const char *response = 
        "HTTP/1.1 200 OK\r\n" 
        "Connection: close\r\n" 
        "Content-Type: text/plain\r\n\r\n" 
        "Local time is: "; 
    int bytes_sent = send(socket_client, response, strlen(response), 0); 
    printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(response));

    //time section
    time_t cur_time;
    time(&cur_time);
    char *time_msg = ctime(&cur_time);
    int byte_sent = send(socket_client, time_msg,strlen(time_msg), 0);

    printf("Closing connection...\n");
    CLOSESOCKET(socket_client);

    printf("Closing listening socket...\n");
    CLOSESOCKET(socket_listen);

    return 0;
}
