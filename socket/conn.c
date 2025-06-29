#include "../proxy/proxy.h"
#include "../utils/helper.h"
#include "../utils/logger.h"
#include "../utils/parser.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void handleClient(int c_fd) {
  char *buffer = (char *)malloc(BUFFER_SIZE);
  int bytes_count = 1;
  size_t curr_ptr = 0;
  while (1) {
    int bytes_read = recv(c_fd, buffer + curr_ptr, BUFFER_SIZE, 0);
    if (bytes_read == 0) {
      infoLog("connection closed by client: %d", c_fd);
      break;
    } else if (bytes_read == -1) {
      errorLog("error reading client request");
      break;
    } else if (bytes_read > 0) {
      curr_ptr += bytes_read;
      // Check if we need to expand buffer
      if (bytes_read == BUFFER_SIZE) {
        buffer = expandMemory(buffer, (++bytes_count * BUFFER_SIZE));
        continue;
      }

      // Null terminate for string operations
      buffer[curr_ptr] = '\0';

      // Check for complete HTTP request
      int is_complete = 0;
      int content_length = 0;

      // Look for \r\n\r\n (end of headers)
      char *headers_end = strstr(buffer, "\r\n\r\n");
      if (headers_end != NULL) {
        // Found end of headers
        int headers_size = headers_end - buffer + 4; // +4 for \r\n\r\n

        // Look for Content-Length header
        char *content_length_header = strstr(buffer, "Content-Length:");
        if (content_length_header != NULL) {
          sscanf(content_length_header, "Content-Length: %d", &content_length);
        }

        // Check if we have complete request
        if (content_length == 0) {
          // No body, request is complete
          is_complete = 1;
        } else {
          // Has body, check if we have all the body data
          size_t total_expected = headers_size + content_length;
          if (curr_ptr >= total_expected) {
            is_complete = 1;
          }
        }
      }

      printf("client [%d]: %s\n", c_fd, buffer);
      printf("Headers end found: %s\n", headers_end ? "YES" : "NO");
      printf("Content-Length: %d\n", content_length);
      printf("Current size: %zu, Complete: %s\n", curr_ptr,
             is_complete ? "YES" : "NO");

      if (is_complete) {
        infoLog("Complete HTTP request received!");
        struct parsed_request *parsed = parseRequest(buffer, curr_ptr);
        if (parsed != NULL) {
          // Use parsed data for proxy forwarding
          char *resp_buff = (char *)malloc(BUFFER_SIZE);
          size_t resp_len = 0;
          forwardRequest((struct parsed_request *)parsed, buffer, curr_ptr,
                         resp_buff, &resp_len);
          // send response back to client
          send(c_fd, resp_buff, resp_len, 0);
          // Free memory when done
          free(resp_buff);
          free(parsed);
          break;
        }
        curr_ptr = 0;
        buffer[0] = '\0';
      }
    } else {
      errorLog("unknown error occured");
    }
  }
  free(buffer);
  close(c_fd);
}