#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "http.h"

int main(void) {
    int error;

    // Create the socket
    int server = socket(AF_INET, SOCK_STREAM, 0);
    if(server == -1) {
        perror("Socket failed to initialize.");
        exit(EXIT_FAILURE);
    }
    
    // Initialize, Bind, and Listen
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    error = bind(server, (struct sockaddr*)&addr, sizeof(addr));
    if(error < 0) {
        close(server);
        perror("Bind to port failed.");
        exit(EXIT_FAILURE);
    }
    error = listen(server, 128);
    if(error < 0) {
        close(server);
        perror("listen to port failed.");
        exit(EXIT_FAILURE);
    }

    // Start accepting requests
    while(1) {
        char buffer[1024];
        HttpRequest req = {0};
        int client = accept(server, NULL, NULL);
        recv(
            client,
            buffer,
            sizeof(buffer),
            0
        );
        parse_http_request(&req, buffer, sizeof(buffer));
        printf("Request Parsed: \nmethod: %s\nuri: %s\nversion : %s\nbody: %zd bytes\n", req.method, req.uri, req.version, req.body_len);

        close(client);
    }

    close(server);
    return 0;
}
