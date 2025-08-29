#ifndef HEADERSCRIPT_H
#define HEADERSCRIPT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "test/test_lib.h"

int HEADERSCRIPT_VM(char* input);

#ifdef __cplusplus
}
#endif

#endif // HEADERSCRIPT_H
#ifdef HEADERSCRIPT_IMPLEMENTATION

unsigned long long hash_c_string(char* s)
{
  unsigned long long h = 14695981039346656037ULL;
  while (*s)
  {
    h = (h ^ *s) * 1099511628211; s += 1;
  } return h;
}


int HEADERSCRIPT_VM(char* input)
{
  char *lines[1024];
  int line_count = 0;
  char *scanner_start, *scanner_current = input;
  while (true)
  {
    scanner_start = scanner_current;
    if (*scanner_current == '\0') break;
    while (*scanner_current != '\0' && *scanner_current != '\n' && *scanner_current != ' ' && *scanner_current != '\t') scanner_current++;
    if (scanner_current > scanner_start) lines[line_count++] = scanner_start;
    while (*scanner_current == '\n' || *scanner_current == ' ' || *scanner_current == '\t' || *scanner_current == '\r')
    {
      *scanner_current = '\0';
      scanner_current++;
    }
  }
  for (int i = 0; i < line_count; i++)
  {
    switch(hash_c_string(lines[i]))
    {
      case 18007334074686647077: test(); break;
    }
  }
}

#endif // HEADERSCRIPT_IMPLEMENTATION
