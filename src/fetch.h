#ifndef FETCH_H
#define FETCH_H
typedef enum {
  FETCH_OK,
  FETCH_ERROR
} fetch_result_t;

extern char *http_read_buffer;

void build_authorization_header(char **header);

size_t http_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);

fetch_result_t fetch(char *url, char **body);
#endif