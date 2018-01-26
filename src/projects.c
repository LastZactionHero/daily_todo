#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "./projects.h"
#include "./fetch.h"

void parse_projects(project_t **head, char *json);
void parse_project(project_t *project, json_t *data);

void load_projects(project_t **head) {
  printf("Fetching Projects...\n");

  char *json = NULL;
  if(FETCH_OK == fetch("https://app.asana.com/api/1.0/projects", &json)) {
    parse_projects(head, json);    
  }
  if(json) {
    free(json);
  }
}

void parse_projects(project_t **head, char *json) {
  json_t *root;
  json_error_t error;
  root = json_loads(json, 0, &error);

  json_t *data = json_object_get(root, "data");
  unsigned int projects_count = json_array_size(data);

  project_t *project_last = NULL;

  for(unsigned int i = 0; i < projects_count; i++ ) {
    project_t *project = NULL;
    if(NULL == *head) {
      *head = calloc(1, sizeof(project_t));
      project = *head;
    } else {
      project = calloc(1, sizeof(project_t));
    }
    json_t *project_data = json_array_get(data, i);
    parse_project(project, project_data);

    if(project_last) {
      project_last->next = project;
    }
    project_last = project;
  }

  json_decref(root);
}

void parse_project(project_t *project, json_t *data) {
  json_t *id, *name;

  id = json_object_get(data, "id");
  json_int_t id_int = json_integer_value(id);
  project->id = id_int;

  name = json_object_get(data, "name");
  const char *name_str = json_string_value(name);

  project->name = malloc(strlen(name_str) + 1);
  strcpy(project->name, name_str);
}

void print_project(project_t *project) {
  printf("%lu: %s\n", project->id, project->name);
}