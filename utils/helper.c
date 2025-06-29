#include "logger.h"
#include <stdlib.h>

void *expandMemory(void *ptr, int exp_size) {
  void *tmp = (void *)realloc(ptr, exp_size);
  if (tmp == NULL) {
    errorLog("error while memory expansion");
    return ptr;
  }
  return tmp;
}