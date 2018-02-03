#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
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
    // Allocate the read buffer
    http_read_buffer = calloc(nmemb + 1, size);
    strncpy(http_read_buffer, ptr, read_bytes);
  } else {
    // Allocate a new read buffer and append
    char *append_read_buffer = calloc(nmemb + strlen(http_read_buffer) + 1, size);

    strncpy(append_read_buffer, http_read_buffer, strlen(http_read_buffer));
    strncpy(&append_read_buffer[strlen(http_read_buffer)], ptr, read_bytes);

    free(http_read_buffer);
    http_read_buffer = append_read_buffer;
  }
  return read_bytes;
}

fetch_result_t fetch(char *url, char **body) {
  fetch_result_t res = FETCH_OK;

  curl_global_init(CURL_GLOBAL_ALL);

  CURL *curl = curl_easy_init();
  if(curl) {    
    curl_easy_setopt(curl, CURLOPT_URL, url);

    struct curl_slist *list = NULL;

    char *authorization_header = NULL;
    build_authorization_header(&authorization_header);
    list = curl_slist_append(list, authorization_header);
    free(authorization_header);

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_write_callback);

    CURLcode curl_res = curl_easy_perform(curl);
    if(CURLE_OK == curl_res) {
      *body = malloc(strlen(http_read_buffer));
      strcpy(*body, http_read_buffer);
    } else {
      res = FETCH_ERROR;
    }
  } else {
    res = FETCH_ERROR;
  }

  if(http_read_buffer) {
    free(http_read_buffer);
    http_read_buffer = NULL;
  }

  curl_easy_cleanup(curl);
  return res;
}