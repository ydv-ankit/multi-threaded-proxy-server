#include "../utils/logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024 * 64 // 64 KB

struct ClientData {
  int client_fd;
  char *buffer;
  size_t current_size;
  size_t capacity;
  int is_complete;
};

void *expandMemory(void *ptr, int exp_size) {
  void *tmp = (void *)realloc(ptr, exp_size);
  if (tmp == NULL) {
    errorLog("error while memory expansion");
    return ptr;
  }
  return tmp;
}

void handleClient(int c_fd) {
  char *buffer = (char *)malloc(BUFFER_SIZE);
  int bytes_count = 1;
  size_t curr_ptr = 0;
  while (1) {
    int bytes_read = recv(c_fd, buffer + curr_ptr, BUFFER_SIZE, 0);
    infoLog("bytes: %d\n", bytes_read);
    if (bytes_read == 0) {
      infoLog("connection closed by client");
      break;
    } else if (bytes_read == -1) {
      errorLog("error reading client request");
      break;
    } else if (bytes_read > 0) {
      if (bytes_read == BUFFER_SIZE) {
        buffer = expandMemory(buffer, (++bytes_count * BUFFER_SIZE));
        curr_ptr += bytes_read;
        continue;
      }
      curr_ptr += bytes_read;
      buffer[curr_ptr + 1] = '\0';
      printf("client [%d]: %s", c_fd, buffer);
    } else {
      errorLog("unknown error occured");
    }
  }
  free(buffer);
}