#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>

struct parsed_request {
    char *method;
    char *path;
    char *http_version;
    struct header *headers;
    char *body;
    size_t body_size;
};

struct header {
  char *key;
  char *value;
  struct header *next_header;
};

struct parsed_request* parseRequest(char *buff, size_t tsize);

#endif