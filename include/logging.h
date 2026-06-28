#ifndef LOGGING_H
#define LOGGING_H

void info(const char *format, ...)    __attribute__((format(printf, 1, 2)));
void warning(const char *format, ...) __attribute__((format(printf, 1, 2)));
void error(const char *format, ...)   __attribute__((format(printf, 1, 2)));
void debug(const char *format, ...)   __attribute__((format(printf, 1, 2)));

#endif