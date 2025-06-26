#include "utils/logger.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX_CONN 10

void initializeSocket(int);
void handleConnection(int, struct sockaddr *, socklen_t *);

void initializeSocket(int port) {
  // socket file descriptor
  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_fd == -1) {
    errorLog("failed to initialize socket");
    exit(1);
  }

  // defining socket structure
  struct sockaddr_in s_addr;
  struct in_addr ia;
  ia.s_addr = INADDR_ANY;
  s_addr.sin_family = AF_INET;
  s_addr.sin_addr = ia;
  s_addr.sin_port = htons(port);

  // bind socket fd to machine network
  int b = bind(sock_fd, (struct sockaddr *)&s_addr, sizeof(s_addr));
  if (b == -1) {
    errorLog("error binding to socket");
    exit(1);
  }
  int l = listen(sock_fd, MAX_CONN);
  if (l == -1) {
    errorLog("error in listening for connections");
    exit(1);
  }
  infoLog("ready to accept connections...");

  // listen for incoming connection requests
  while (1) {
    struct sockaddr *client_addr;
    socklen_t *client_addr_len = NULL;
    int conn_fd = accept(sock_fd, client_addr, client_addr_len);
    handleConnection(conn_fd, client_addr, client_addr_len);
  }
}