#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./fetch.h"

char *http_read_buffer = NULL;

void build_authorization_header(char **header) {
  const char* prefix = "Authorization: Bearer ";
  const char* token = getenv("ASANA_DAILY_TODO_TOKEN");

  *header = malloc(strlen(prefix) + strlen(token) + 1);
  sprintf(*header, "%s%s", prefix, token);
}

size_t http_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
  size_t read_bytes = size * nmemb;
  if(!http_read_buffer) {
    http_read_buffer = calloc(nmemb + 1, size);
    strncpy(http_read_buffer, ptr, read_bytes);
  }
  return read_bytes;
}
