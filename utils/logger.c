#include <stdarg.h>
#include <stdio.h>

void errorLog(const char *format, ...) {
  va_list args;
  va_start(args, format);
  printf("ERROR: ");
  vprintf(format, args);
  printf("\n");
  va_end(args);
}

void infoLog(const char *format, ...) {
  va_list args;
  va_start(args, format);
  printf("INFO: ");
  vprintf(format, args);
  printf("\n");
  va_end(args);
}
