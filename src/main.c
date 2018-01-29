#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./projects.h"

const char* PROJECT_WHITELIST_FILENAME = "./project_whitelist.txt";

project_t *projects_head = NULL;

void load_project_whitelist(unsigned long int **whitelist) {
  FILE *fp;
  fp = fopen(PROJECT_WHITELIST_FILENAME, "r");
  if(!fp) {
    return;
  }

  fseek(fp, 0L, SEEK_END);
  long sz = ftell(fp);
  if(sz <= 0) {
    return;
  }
  rewind(fp);

  char *read_buffer = calloc(sizeof(char), sz + 1);
  fread(read_buffer, 1, sz, fp);
  fclose(fp);

  // Count the number of lines in the file
  unsigned int line_count = 1;
  for(unsigned  i = 0; i < sz; i++) {
    if(read_buffer[i] == '\n') {
      line_count++;
    }
  }

  *whitelist = calloc(sizeof(unsigned long int), line_count + 1);  

  char *line = strtok(read_buffer, "\n");
  unsigned int cur_line = 0;
  while(line) {
    unsigned long int id = atol(line);
    (*whitelist)[cur_line] = id;
    cur_line++;
    line = strtok(NULL, "\n");
  }

  free(read_buffer);
}

int main() {  
  load_projects(&projects_head);

  unsigned long int *project_id_whitelist;
  load_project_whitelist(&project_id_whitelist);  
  whitelist_projects(projects_head, project_id_whitelist);
  free(project_id_whitelist);

  project_t *project = projects_head;
  while(project) {        
    if(project->whitelisted) {
      print_project(project);
      load_project_tasks(project);
    }    
    project = project->next;
  }
  return 0;
}

// char *http_read_buffer = NULL;
// workspace_t *workspace_head = NULL;

// size_t http_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
//   size_t read_bytes = size * nmemb;
//   if(!http_read_buffer) {
//     http_read_buffer = calloc(nmemb + 1, size);
//     strncpy(http_read_buffer, ptr, read_bytes);
//   }
//   return read_bytes;
// }

// void parse_workspace(workspace_t *ws, json_t *data) {
//   json_t *id, *name;
  
//   id = json_object_get(data, "id");
//   json_int_t id_int = json_integer_value(id);
//   ws->id = id_int;

//   name = json_object_get(data, "name");
//   const char *name_str = json_string_value(name);

//   ws->name = malloc(strlen(name_str) + 1);
//   strcpy(ws->name, name_str);
// }

// typedef enum {
//   FETCH_OK,
//   FETCH_ERROR
// } fetch_result_t;

// void build_authorization_header(char **header) {
//   const char* prefix = "Authorization: Bearer ";
//   const char* token = getenv("ASANA_DAILY_TODO_TOKEN");

//   *header = malloc(strlen(prefix) + strlen(token) + 1);
//   sprintf(*header, "%s%s", prefix, token);
// }

// int fetch_workspaces(char **json) {
//   fetch_result_t res = FETCH_OK;

//   curl_global_init(CURL_GLOBAL_ALL);

//   CURL *curl = curl_easy_init();
//   if(curl) {    
//     curl_easy_setopt(curl, CURLOPT_URL, "https://app.asana.com/api/1.0/workspaces");

//     struct curl_slist *list = NULL;

//     char *authorization_header = NULL;
//     build_authorization_header(&authorization_header);
//     list = curl_slist_append(list, authorization_header);
//     free(authorization_header);

//     curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

//     curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_write_callback);

//     CURLcode curl_res = curl_easy_perform(curl);
//     if(CURLE_OK == curl_res) {
//       *json = malloc(strlen(http_read_buffer));
//       strcpy(*json, http_read_buffer);
//     } else {
//       res = FETCH_ERROR;
//     }
//   } else {
//     res = FETCH_ERROR;
//   }

//   if(http_read_buffer) {
//     free(http_read_buffer);
//     http_read_buffer = NULL;
//   }

//   curl_easy_cleanup(curl);
//   return res;
// }

// int load_workspaces(workspace_t **head) {
//   char *json = NULL;

//   if(fetch_workspaces(&json) == FETCH_OK) {
//     json_t *root;
//     json_error_t error;
//     root = json_loads(json, 0, &error);
//     free(json);

//     json_t *data = json_object_get(root, "data");

//     uint workspace_count = json_array_size(data);

//     workspace_t *ws_last = NULL;

//     for(uint i = 0; i < workspace_count; i++ ) {
//       workspace_t *ws = NULL;
//       if(NULL == *head) {
//         *head = calloc(1, sizeof(workspace_t));
//         ws = *head;
//       } else {
//         ws = calloc(1, sizeof(workspace_t));
//       }
//       json_t *workspace_data = json_array_get(data, i);
//       parse_workspace(ws, workspace_data);

//       if(ws_last) {
//         ws_last->next = ws;
//       }
//       ws_last = ws;
//     }
//   } else {
//     fprintf(stderr, "An error occurred downloading");
//     return -1;
//   }
//   return 0;
// }

// void free_workspaces(workspace_t *workspace) {
//   workspace_t *ws_next;

//   while(workspace) {
//     ws_next = workspace->next;

//     free(workspace->name);
//     free(workspace);
//     workspace = ws_next;
//   }
// }

// int main() {
//   load_workspaces(&workspace_head);
  
//   workspace_t *ws = workspace_head;
//   while(ws) {
//     printf("%lu\t%s\n", ws->id, ws->name);
//     ws = ws->next;
//   }

//   free_workspaces(workspace_head);

//   return 0;
// }
