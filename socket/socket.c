#include "../utils/logger.h"
#include "conn.h"
#include <netinet/in.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_CONN 10 // max connections in pending queue

void *useThread(void *arg) {
  int client_fd = *(int *)arg;
  handleClient(client_fd);
  free(arg);
  return NULL;
}

void initializeSocket(int port) {
  // socket file descriptor
  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_fd == -1) {
    errorLog("failed to initialize socket");
    exit(1);
  }

  // set socket options to allow address reuse
  int opt = 1;
  if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    errorLog("failed to set socket options");
    close(sock_fd);
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

  // continuously listen for incoming connection requests
  while (1) {
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int conn_fd =
        accept(sock_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (conn_fd == -1) {
      errorLog("error accepting connection request from client");
      continue;
    }
    infoLog("got client connection: %d", conn_fd);
    pthread_t thread;
    int *client_fd = malloc(sizeof(int));
    *client_fd = conn_fd;
    int p = pthread_create(&thread, NULL, &useThread, client_fd);
    if (p != 0) {
      errorLog("cannot handle client %d connection request", conn_fd);
      continue;
    }
    pthread_detach(thread);
  }
}