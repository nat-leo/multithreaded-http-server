#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include "http.h"

ssize_t request_line_end(char *buffer, size_t len) {
    for(size_t i=0; i < len-4; i++) {
        if(buffer[i] == '\r' && buffer[i+1] == '\n' && buffer[i+2] == '\r' && buffer[i+3] == '\n') {
            return i;
        }
    }
    return -1;
}

int parse_http_request(HttpRequest *req, char *buffer) {
    int matched = sscanf(buffer, "%8s %8000s %8s", req->method, req->uri, req->version);
    if(matched != 3) {
        fprintf(stderr, "Parsing failed.");
        return -1;
    }
    matched = sscanf(buffer, "Content-Length: %zd", &(req->body_len));
    if(matched == 0) {
        req->body_len = 0;
    }

    char *headers = strstr(buffer, "\r\n");
    char *line = strtok(headers, "\r\n");
    while(line != NULL) {
        line = strtok(NULL, "\r\n");
    }

    return 0;
}

int get(HttpRequest *req, int client_socket) {
    struct stat st;
    char path[512]; 
    int result = snprintf(path, sizeof(path), "public/%s", req->uri);
    if(result < 0) {
        fprintf(stderr, "Error creating path %s using %s", path, req->uri);
    }
    int file = open(path, O_RDONLY);
    if(file < 0) {
        fprintf(stderr, "Error reading file %s", req->uri);
    } else {
        fstat(file, &st);

        char header[1024];
        snprintf(
            header,
            sizeof(header),
            "HTTP/1.1 200 OK\r\n"
            "Content-Length: %lld\r\n"
            "Content-Type: text/plain\r\n"
            "\r\n",
            (long long)st.st_size
        );

        send(client_socket, header, strlen(header), 0);
    }

    char buffer[4096];
    ssize_t read_bytes;
    while( (read_bytes = read(file, buffer, sizeof(buffer)) ) > 0) {
        fprintf(stdout, "%s", buffer);
        ssize_t bytes_sent = 0;
        while(bytes_sent < read_bytes) {
            ssize_t send_bytes = send(
                client_socket, 
                buffer + bytes_sent, 
                read_bytes - bytes_sent, 
                0
            );
            bytes_sent += send_bytes;
        }   
    }
    close(file);

    return 0;
}

int put(HttpRequest *req, int client_socket) {
    struct stat st;
    char path[512]; 
    int result = snprintf(path, sizeof(path), "public/%s", req->uri);
    if(result < 0) {
        fprintf(stderr, "Error creating path %s using %s", path, req->uri);
    }
    int file = open(path, O_RDONLY);
    if(file < 0) {
        fprintf(stderr, "Error reading file %s", req->uri);
    } else {
        fstat(file, &st);

        char header[1024];
        snprintf(
            header,
            sizeof(header),
            "HTTP/1.1 200 OK\r\n"
            "Content-Length: %lld\r\n"
            "Content-Type: text/plain\r\n"
            "\r\n",
            (long long)st.st_size
        );

        send(client_socket, header, strlen(header), 0);
    }

    char buffer[4096];
    ssize_t read_bytes;
    while( (read_bytes = read(file, buffer, sizeof(buffer)) ) > 0) {
        fprintf(stdout, "%s", buffer);
        ssize_t bytes_sent = 0;
        while(bytes_sent < read_bytes) {
            ssize_t send_bytes = send(
                client_socket, 
                buffer + bytes_sent, 
                read_bytes - bytes_sent, 
                0
            );
            bytes_sent += send_bytes;
        }   
    }
    close(file);
    return 0;
}


// make http
// int main(void) {
//     char sample_http_buffer[] = "GET /api/v1/users/42 HTTP/1.1\r\nHost: www.example.com\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\nAccept: application/json\r\nAccept-Language: en-US\r\nConnection: keep-alive\r\n\r\nhello world";
//     HttpRequest req = {0};

//     ssize_t last_request_line_byte = request_line_end(sample_http_buffer, sizeof(sample_http_buffer));

//     ssize_t error = parse_request_line(&req, sample_http_buffer, last_request_line_byte);
//     if(error < 0) {
//         printf("Failed to parse request line");
//         return -1;
//     }

//     if(last_request_line_byte < 0) {
//         printf("Parsing http request failed.\n");
//         return -1;
//     }

//     //HttpRequest req = {0};
//     //int error = parse_headers(&req, sample_http_buffer, sizeof(sample_http_buffer));

//     printf("Request Parsed: \nmethod: %s\nuri: %s\nversion : %s\nbody: %zd bytes\n", req.method, req.uri, req.version, req.body_len);

//     return 0;
// }
