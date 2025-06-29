#ifndef PROXY_H
#define PROXY_H
#include "../utils/parser.h"

void forwardRequest(struct parsed_request *, char*, int, char *, size_t*);

#endif