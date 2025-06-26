#include "socket/socket.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <port>\n", argv[0]);
    return 1;
  }
  int port = atoi(argv[1]);
  initializeSocket(port);

  return 0;
}