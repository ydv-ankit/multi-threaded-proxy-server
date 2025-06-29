#include "../utils/helper.h"
#include "../utils/logger.h"
#include "../utils/parser.h"
#include <netdb.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void receiveResponse(int tar_con_fd, char *resp_buff, size_t *resp_buff_len) {
  size_t curr_ptr = 0;
  int bytes_count = 0;
  int chunked = 0; // false initially
  while (1) {
    int bytes_read = recv(tar_con_fd, resp_buff + curr_ptr, BUFFER_SIZE, 0);
    if (bytes_read == 0) {
      infoLog("response received from client: %d", tar_con_fd);
      break;
    } else if (bytes_read == -1) {
      errorLog("failed to receive target response");
      break;
    } else if (bytes_read > 0) {
      curr_ptr += bytes_read;
      // Check if we need to expand buffer
      if (bytes_read == BUFFER_SIZE) {
        resp_buff = expandMemory(resp_buff, (++bytes_count * BUFFER_SIZE));
        continue;
      }

      // Null terminate for string operations
      resp_buff[curr_ptr] = '\0';

      // Check for complete HTTP request
      int is_complete = 0;
      int content_length = 0;

      // Look for \r\n\r\n (end of headers)
      char *headers_end = strstr(resp_buff, "\r\n\r\n");
      if (headers_end != NULL) {
        // Found end of headers
        int headers_size = headers_end - resp_buff + 4; // +4 for \r\n\r\n

        // Look for Content-Length header
        char *content_length_header = strstr(resp_buff, "Content-Length:");
        if (content_length_header != NULL) {
          sscanf(content_length_header, "Content-Length: %d", &content_length);
        }
        char *transfer_encoding = strstr(resp_buff, "Transfer-Encoding:");
        if (transfer_encoding != NULL) {
          chunked = 1;
        }

        // Check if we have complete request
        if (content_length == 0) {
          // No body, request is complete
          is_complete = 1;
        } else {
          // Has body, check if we have all the body data
          size_t total_expected = headers_size + content_length;
          if (curr_ptr >= total_expected && chunked == 0) {
            is_complete = 1;
          }
        }
      }

      printf("client [%d]: %s\n", tar_con_fd, resp_buff);
      printf("Headers end found: %s\n", headers_end ? "YES" : "NO");
      printf("Content-Length: %d\n", content_length);
      printf("Current size: %zu, Complete: %s\n", curr_ptr,
             is_complete ? "YES" : "NO");

      if (is_complete && chunked == 0) {
        infoLog("Response received!");
        break;
      }
    } else {
      errorLog("unknown error occured");
      break;
    }
  }
  printf("RESPONSE: %s", resp_buff);
  *resp_buff_len = curr_ptr;
}

void forwardRequest(struct parsed_request *req, char *req_buff,
                    int req_buff_len, char *resp_buff, size_t *resp_len) {
  char *host = NULL;
  char *port = NULL;
  // host and port
  struct header *cur = req->headers;
  while (cur) {
    if (strcmp(cur->key, "Host") == 0) {
      host = cur->value;
      break;
    }
    cur = cur->next_header;
  }
  if (!host) {
    errorLog("cannot find host");
    return;
  }
  int tar_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (tar_sock_fd == -1) {
    errorLog("failed to create socket for target server");
    return;
  }
  char *ptr = strchr(host, (int)':');
  if (ptr != NULL) {
    port = ptr + 1;
    *ptr = '\0';
  } else {
    port = "80";
  }

  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = 0;

  struct addrinfo *res;
  int result = getaddrinfo(host, port, &hints, &res);
  if (result != 0) {
    errorLog("failed to resolve host");
    return;
  }
  int conn_res = connect(tar_sock_fd, res->ai_addr, res->ai_addrlen);
  if (conn_res == -1) {
    errorLog("failed connecting to host! retrying...");
    return;
  }
  send(tar_sock_fd, req_buff, req_buff_len, 0);
  printf("\n\nREQUEST: %s\n", req_buff);
  receiveResponse(tar_sock_fd, resp_buff, resp_len);
  close(tar_sock_fd);
  freeaddrinfo(res);
}
