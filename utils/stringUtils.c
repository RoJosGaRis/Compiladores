#include "stringUtils.h"

char* unescape_string(const char* input) {
  size_t len = strlen(input);
  char* output = malloc(len + 1); // máximo igual de largo
  if (!output) return NULL;

  char* dst = output;
  const char* src = input;

  while (*src) {
      if (*src == '\\') {
          src++;
          switch (*src) {
              case 'n': *dst++ = '\n'; break;
              case 't': *dst++ = '\t'; break;
              case '\\': *dst++ = '\\'; break;
              case '\"': *dst++ = '\"'; break;
              default: *dst++ = *src; break;  // fallback
          }
          src++;
      } else {
          *dst++ = *src++;
      }
  }
  *dst = '\0';
  return output;
}