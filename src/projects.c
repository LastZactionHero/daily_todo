#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "./projects.h"
#include "./fetch.h"

fetch_result_t fetch_projects(char **json);

void load_projects() {
  char *json = NULL;
  printf("Fetching Projects...\n");
  fetch_projects(&json);
  free(json);
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