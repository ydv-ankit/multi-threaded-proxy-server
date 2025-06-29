#include "parser.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct parsed_request *parseRequest(char *buff, size_t tsize) {
  printf("Parsing HTTP request...\n\n");

  // method
  size_t t_count = 0;
  char *method = buff;
  size_t curr_ptr = 0;
  while (buff[curr_ptr] != ' ')
    curr_ptr++;
  buff[curr_ptr++] = '\0';
  t_count = curr_ptr;

  printf("method: '%s'\n", method);

  // path
  char *path = &buff[t_count];
  curr_ptr = 0;
  while (buff[t_count + curr_ptr] != ' ')
    curr_ptr++;
  buff[t_count + curr_ptr++] = '\0';
  printf("path: '%s'\n", path);
  t_count += curr_ptr;

  // HTTP version
  char *http_ver = &buff[t_count];
  curr_ptr = 0;
  while (buff[t_count + curr_ptr] != '\r')
    curr_ptr++;
  buff[t_count + curr_ptr++] = '\0';
  printf("http version: '%s'\n", http_ver);
  t_count += curr_ptr;

  // Skip \r\n after request line
  t_count += 1; // skip \n

  // Parse headers
  struct header *headers = NULL;
  struct header *last = NULL;

  while (t_count < tsize &&
         !(buff[t_count] == '\r' && buff[t_count + 1] == '\n')) {
    struct header *h = malloc(sizeof(struct header));
    if (!h) {
      fprintf(stderr, "Header allocation failed\n");
      break;
    }

    h->key = &buff[t_count];
    curr_ptr = 0;
    while (buff[t_count + curr_ptr] != ':')
      curr_ptr++;
    buff[t_count + curr_ptr++] = '\0';

    // skip space after colon
    if (buff[t_count + curr_ptr] == ' ')
      curr_ptr++;

    h->value = &buff[t_count + curr_ptr];
    while (buff[t_count + curr_ptr] != '\r')
      curr_ptr++;
    buff[t_count + curr_ptr++] = '\0';

    // Skip \n
    t_count += curr_ptr + 1;

    // link the header
    h->next_header = NULL;
    if (last) {
      last->next_header = h;
    } else {
      headers = h;
    }
    last = h;
  }

  // Skip final \r\n after headers
  t_count += 2;

  // Print headers
  struct header *cur = headers;
  while (cur) {
    printf("Header: '%s' -> '%s'\n", cur->key, cur->value);
    cur = cur->next_header;
  }

  // Body
  char *body = &buff[t_count];
  if (t_count < tsize) {
    body[tsize - t_count] = '\0';
    printf("Body: %s\n", body);
  }

  struct parsed_request *parsed =
      (struct parsed_request *)malloc(sizeof(struct parsed_request));
  if (!parsed) {
    fprintf(stderr, "Parsed request allocation failed\n");
    return NULL;
  }

  parsed->method = method;
  parsed->path = path;
  parsed->http_version = http_ver;
  parsed->headers = headers;
  parsed->body = (t_count < tsize) ? body : NULL;
  parsed->body_size = tsize - t_count;
  printf("Parsing complete!\n");

  return parsed;
}
