#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <string.h>
#include <stdio.h>
int main(void)
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(443);
    inet_pton(AF_INET, "142.250.68.14", &addr.sin_addr);
    connect(socket_fd, (struct sockaddr *)&addr, sizeof(addr));

    SSL_CTX *ctx = SSL_CTX_new(TLS_method());
    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, socket_fd);
    SSL_set_tlsext_host_name(ssl, "www.google.com");
    SSL_connect(ssl);
    char *request = "GET / HTTP/1.1\r\nHost: www.google.com\r\nConnection: close\r\n\r\n";
    SSL_write(ssl, request, strlen(request));
    char buffer[1024] = {0};
    SSL_read(ssl, buffer, 1023);
    printf("Response:\n%s\n", buffer);
}