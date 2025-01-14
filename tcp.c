#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>

#define ISVALIDSOCKET(s) ((s) >= 0) // Valid socket check
#define GETSOCKETERRNO() (errno)    // Get socket error
#define CLOSESOCKET(s) (close(s))  // Cross-platform close socket

int main() {
    struct addrinfo hints, *bind_address = NULL;
    int socket_listen = -1, socket_client = -1; // Initialize socket descriptors

    // Initialize 'hints' structure
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_flags = AI_PASSIVE;     // Bind to local address
    hints.ai_socktype = SOCK_STREAM; // TCP

    // Get address info
    const char *host = NULL; // NULL for local machine
    const char *service = "8080";
    if (getaddrinfo(host, service, &hints, &bind_address) != 0) {
        fprintf(stderr, "getaddrinfo() failed [%s]\n", strerror(GETSOCKETERRNO()));
        goto cleanup;
    }

    printf("Creating a socket...\n");
    socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
    if (!ISVALIDSOCKET(socket_listen)) {
        fprintf(stderr, "socket() failed [%s]\n", strerror(GETSOCKETERRNO()));
        goto cleanup;
    }

    printf("Binding socket to local address...\n");
    if (bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)) {
        fprintf(stderr, "bind() failed [%s]\n", strerror(GETSOCKETERRNO()));
        goto cleanup;
    }

    freeaddrinfo(bind_address); 
    bind_address = NULL;        


    printf("Listening on port %s...\n", service);
    if (listen(socket_listen, 10) < 0) {
        fprintf(stderr, "listen() failed [%s]\n", strerror(GETSOCKETERRNO()));
        goto cleanup;
    }

    printf("Waiting for a client to connect...\n");
    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    socket_client = accept(socket_listen, (struct sockaddr *)&client_address, &client_len);
    if (!ISVALIDSOCKET(socket_client)) {
        fprintf(stderr, "accept() failed [%s]\n", strerror(GETSOCKETERRNO()));
        goto cleanup;
    }

    printf("Client connected: ");
    char addr_buffer[100];
    getnameinfo((struct sockaddr *)&client_address, client_len,
                addr_buffer, sizeof(addr_buffer), NULL, 0, NI_NUMERICHOST);
    printf("%s\n", addr_buffer);


    printf("Reading request...\n");
    char request[1024];
    int bytes_received = recv(socket_client, request, sizeof(request), 0);
    if (bytes_received < 0) {
        fprintf(stderr, "recv() failed [%s]\n", strerror(GETSOCKETERRNO()));
        goto cleanup;
    }
    printf("Received %d bytes:\n%.*s\n", bytes_received, bytes_received, request);

    
    printf("Sending response...\n");
    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Connection: close\r\n"
        "Content-Type: text/plain\r\n\r\n"
        "Local time is: ";
    int bytes_sent = send(socket_client, response, strlen(response), 0);
    if (bytes_sent < 0) {
        fprintf(stderr, "send() failed [%s]\n", strerror(GETSOCKETERRNO()));
        goto cleanup;
    }
    printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(response));

    
    time_t cur_time;
    time(&cur_time);
    char *time_msg = ctime(&cur_time);
    bytes_sent = send(socket_client, time_msg, strlen(time_msg), 0);
    if (bytes_sent < 0) {
        fprintf(stderr, "send() failed [%s]\n", strerror(GETSOCKETERRNO()));
        goto cleanup;
    }
    printf("Sent time: %s\n", time_msg);

cleanup:
    if (bind_address) freeaddrinfo(bind_address);
    if (ISVALIDSOCKET(socket_client)) CLOSESOCKET(socket_client);
    if (ISVALIDSOCKET(socket_listen)) CLOSESOCKET(socket_listen);

    printf("Exiting.\n");
    return 0;
}
