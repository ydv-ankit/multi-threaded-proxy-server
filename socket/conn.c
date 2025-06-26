#include "../utils/logger.h"
#include <stdio.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024 * 512 // 512 KB

void handleClient(int c_fd) {
  char buffer[BUFFER_SIZE];
  while (1) {
    int bytes_read = recv(c_fd, buffer, sizeof(buffer), 0);
    infoLog("bytes: %d", bytes_read);
    if (bytes_read == 0) {
      infoLog("connection closed by client");
      return;
    } else if (bytes_read == -1) {
      errorLog("error reading client request");
      return;
    } else if (bytes_read > 0) {
      buffer[bytes_read] = '\0';
      printf("client [%d]: %s\n", c_fd, buffer);
    } else {
      errorLog("unknown error occured");
    }
  }
}