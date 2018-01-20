#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <jansson.h>

typedef struct {
  char *id;
  char *name;
} task;

typedef struct {
  char *id;
  char *name;
  task *tasks;
} project;

typedef struct {
  unsigned long int id;
  char *name;
  project *projects;
} workspace_t;


char *http_read_buffer = NULL;

workspace_t *workspaces;

size_t workspace_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
  printf("in write callback...");
  printf("%lu\n", size);
  printf("%lu\n", nmemb);

  size_t read_bytes = size * nmemb;
  if(!http_read_buffer) {
    http_read_buffer = calloc(nmemb + 1, size);
    strncpy(http_read_buffer, ptr, read_bytes);
  }
  printf("%s\n", http_read_buffer);
  return read_bytes;
}

void parse_workspace(workspace_t *ws, json_t *data) {
  json_t *id, *name;
  
  id = json_object_get(data, "id");
  json_int_t id_int = json_integer_value(id);
  ws->id = id_int;

  name = json_object_get(data, "name");
  const char *name_str = json_string_value(name);

  ws->name = malloc(strlen(name_str) + 1);
  strcpy(ws->name, name_str);
}

int fetch_workspaces(workspace_t **workspaces) {
  curl_global_init(CURL_GLOBAL_ALL);

  CURL *curl = curl_easy_init();
  if(curl) {    
    curl_easy_setopt(curl, CURLOPT_URL, "https://app.asana.com/api/1.0/workspaces");

    struct curl_slist *list = NULL;
    list = curl_slist_append(list, "Authorization: Bearer 0/8b82d327dee3f3d31bc6afa6575a5a54");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, workspace_write_callback);

    CURLcode res = curl_easy_perform(curl);
    printf("res: %d\n", res);
    if(res == CURLE_OK) {
      json_t *root;
      json_error_t error;
      root = json_loads(http_read_buffer, 0, &error);
      free(http_read_buffer);

      json_t *data = json_object_get(root, "data");

      uint workspace_count = json_array_size(data);
      *workspaces = calloc(workspace_count + 1, sizeof(workspace_t));


      printf("Workspace count: %d\n", workspace_count);
      for(uint i = 0; i < workspace_count; i++ ) {
        json_t *workspace_data = json_array_get(data, i);
        parse_workspace(&(*workspaces)[i], workspace_data);
      }
    } else {
      fprintf(stderr, "An error occurred downloading: %d", res);
      return -1;
    }

    curl_easy_cleanup(curl);
  }
  return 0;
}

int main() {
  fetch_workspaces(&workspaces);

  printf("*workspaces: %lu\n", sizeof(*workspaces));
  printf("workspaces[0]: %lu\n", sizeof(workspaces[0]));
  printf("workspace_t: %lu\n", sizeof(workspace_t));
  uint ws_id = 0;
  workspace_t *ws;
  ws = &workspaces[ws_id];
  while(ws) {
    ws_id++;

    printf("IDX: %d\n", ws_id);
    printf("ID: %lu\n", ws->id);
    printf("Name: %s\n", ws->name);

    ws = &workspaces[ws_id];

    if(ws_id >= 7) {
      break;
    }
  }
  return 0;
}
