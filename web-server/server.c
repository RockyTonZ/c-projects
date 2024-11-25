#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <netinet/in.h>
int main(void)
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr =
        {
            AF_INET,
            0x901f,
            0,
        };
    bind(socket_fd, &addr, sizeof(addr));

    listen(socket_fd, 10);

    int client_fd = accept(socket_fd, 0, 0);

    // The clients message will be stored in this buffer of 256 bytes
    char buffer[256] = {0};
    recv(client_fd, buffer, 256, 0);

    // The buffer is gonna look like this 'GET /file.html .....' nad we only want the file.html part so in the line below
    // we skip 5 characters (this code only supports GET)
    char *file = buffer + 5;

    // Then in here strchr eill look for s given character and returns the pointer to that character
    // so we look for an space and set its pointer to 0 so we remove the rest of the string
    *strchr(file, ' ') = 0;

    // Open the file.html using open() function
    int opened_fd = open(file, O_RDONLY);

    // Here with sendfile() function we can transfer data from a file descriptor to another fd
    // Now we want to transfer the data that we have in the server to clients file descriptor
    sendfile(client_fd, opened_fd, 0, 256);
    close(opened_fd);
    close(client_fd);
    close(socket_fd);
}

// Link to the video that I wrote the code from: https://www.youtube.com/watch?v=2HrYIl6GpYg&t=115s