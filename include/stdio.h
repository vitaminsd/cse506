#ifndef _STDIO_H
#define _STDIO_H

#include <unistd.h>

int scanf(const char *format, ...);
void putchar(unsigned char c);
void puts(unsigned char* str);
void printf(const char *format,...);
int hello_world(char* temp);
#endif
