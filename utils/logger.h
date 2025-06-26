#ifndef LOGGER_H
#define LOGGER_H

#include <stdarg.h>

void errorLog(const char *format, ...);
void infoLog(const char *format, ...);

#endif