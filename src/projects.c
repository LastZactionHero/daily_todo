#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "./projects.h"
#include "./fetch.h"

fetch_result_t fetch_projects(char **json);
void build_authorization_header(char **header);

void load_projects() {
  char *json = NULL;
  fetch_projects(&json);

  
  printf("Loading Projects...\n");
}

char *http_read_buffer = NULL;

size_t http_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
  size_t read_bytes = size * nmemb;
  if(!http_read_buffer) {
    http_read_buffer = calloc(nmemb + 1, size);
    strncpy(http_read_buffer, ptr, read_bytes);
  }
  return read_bytes;
}

void build_authorization_header(char **header) {
  const char* prefix = "Authorization: Bearer ";
  const char* token = getenv("ASANA_DAILY_TODO_TOKEN");

  *header = malloc(strlen(prefix) + strlen(token) + 1);
  sprintf(*header, "%s%s", prefix, token);
}

fetch_result_t fetch_projects(char **json) {
  fetch_result_t res = FETCH_OK;

  curl_global_init(CURL_GLOBAL_ALL);

  CURL *curl = curl_easy_init();
  if(curl) {    
    curl_easy_setopt(curl, CURLOPT_URL, "https://app.asana.com/api/1.0/projects");

    struct curl_slist *list = NULL;

    char *authorization_header = NULL;
    build_authorization_header(&authorization_header);
    list = curl_slist_append(list, authorization_header);
    free(authorization_header);

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_write_callback);

    CURLcode curl_res = curl_easy_perform(curl);
    if(CURLE_OK == curl_res) {
      *json = malloc(strlen(http_read_buffer));
      strcpy(*json, http_read_buffer);
      printf("%s\n", *json);
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