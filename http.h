#pragma once

typedef struct HttpHeader {
    char name[64];
    char value[128];
} HttpHeader;

typedef struct HttpRequest {
    // maximum number of bytes in the request line is 8,192 bytes.
    char method[8]; // between 3 and 7 bytes.
    char uri[8000];
    char version[9]; // HTTP/1.0, HTTP/1.1, etc.
    HttpHeader headers[2048];
    size_t header_len;
    char* body;
    ssize_t content_length;
} HttpRequest;

int parse_http_request(HttpRequest* req, char *buffer);

int get(HttpRequest* req, int client_socket);

int put(HttpRequest* req, int client_socket, char* buffer, ssize_t buffer_size, ssize_t n);

