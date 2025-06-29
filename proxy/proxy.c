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
  int headers_complete = 0;
  size_t headers_end_pos = 0;

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

      resp_buff[curr_ptr] = '\0';

      // First, find the end of headers if we haven't already
      if (!headers_complete) {
        char *headers_end = strstr(resp_buff, "\r\n\r\n");
        if (headers_end != NULL) {
          headers_end_pos = headers_end - resp_buff + 4; // +4 for \r\n\r\n
          headers_complete = 1;

          // Check for Transfer-Encoding: chunked
          char *transfer_encoding = strstr(resp_buff, "Transfer-Encoding:");
          if (transfer_encoding != NULL &&
              strstr(transfer_encoding, "chunked") != NULL) {
            chunked = 1;
            infoLog("Detected chunked transfer encoding");
          }
        }
      }

      // handle different response types
      if (headers_complete) {
        if (chunked) {
          // Look for the final chunk marker: 0\r\n\r\n
          char *final_chunk =
              strstr(resp_buff + headers_end_pos, "\r\n0\r\n\r\n");
          if (final_chunk != NULL) {
            infoLog("Complete chunked response received!");
            break;
          }
        } else {
          int content_length = 0;
          char *content_length_header = strstr(resp_buff, "Content-Length:");
          if (content_length_header != NULL) {
            sscanf(content_length_header, "Content-Length: %d",
                   &content_length);
          }

          if (content_length == 0) {
            infoLog("Response received!");
            break;
          } else {
            size_t total_expected = headers_end_pos + content_length;
            if (curr_ptr >= total_expected) {
              infoLog("Response received!");
              break;
            }
          }
        }
      }
    } else {
      errorLog("unknown error occured");
      break;
    }
  }
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

  // Reconstruct the HTTP request with proper path
  char *new_req_buff = malloc(BUFFER_SIZE);
  if (!new_req_buff) {
    errorLog("failed to allocate memory for new request");
    close(tar_sock_fd);
    freeaddrinfo(res);
    return;
  }

  // Extract path from the full URL
  char *path = req->path;
  if (strncmp(path, "http://", 7) == 0) {
    // Skip "http://"
    path += 7;
    // Find the first '/' after the host
    char *path_start = strchr(path, '/');
    if (path_start) {
      path = path_start;
    } else {
      path = "/";
    }
  } else if (strncmp(path, "https://", 8) == 0) {
    // Skip "https://"
    path += 8;
    // Find the first '/' after the host
    char *path_start = strchr(path, '/');
    if (path_start) {
      path = path_start;
    } else {
      path = "/";
    }
  }

  // Build the new request
  int offset = 0;
  offset += snprintf(new_req_buff + offset, BUFFER_SIZE - offset,
                     "%s %s %s\r\n", req->method, path, req->http_version);

  // Add headers
  cur = req->headers;
  while (cur) {
    offset += snprintf(new_req_buff + offset, BUFFER_SIZE - offset,
                       "%s: %s\r\n", cur->key, cur->value);
    cur = cur->next_header;
  }

  // Add final \r\n
  offset += snprintf(new_req_buff + offset, BUFFER_SIZE - offset, "\r\n");

  // Add body if present
  if (req->body && req->body_size > 0) {
    if (offset + req->body_size < BUFFER_SIZE) {
      memcpy(new_req_buff + offset, req->body, req->body_size);
      offset += req->body_size;
    }
  }

  send(tar_sock_fd, new_req_buff, offset, 0);
  receiveResponse(tar_sock_fd, resp_buff, resp_len);
  free(new_req_buff);
  close(tar_sock_fd);
  freeaddrinfo(res);
}
