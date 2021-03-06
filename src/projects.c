#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "./projects.h"
#include "./fetch.h"
#include "./tasks.h"

project_t* parse_projects(char *json);
void parse_project(project_t *project, json_t *data);

void whitelist_projects(project_t *projects_head, unsigned long int *whitelist_ids) {
  project_t *project = projects_head;
  while(project) {
    unsigned int whitelist_ctr = 0;
    while(*(whitelist_ids+whitelist_ctr)) {
      if(project->id == *(whitelist_ids+whitelist_ctr)) {
        project->whitelisted = true;
      }
      whitelist_ctr++;
    }    
    project = project->next;
  }
}

project_t* load_projects() {
  project_t *head = NULL;
  char *json = NULL;

  if(FETCH_OK == fetch("https://app.asana.com/api/1.0/projects", &json)) {
    head = parse_projects(json);
  }
  if(json) {
    free(json);
  }

  return head;
}

void load_project_tasks(project_t *project) {
  char *json = NULL;

  const char project_tasks_path[] = "https://app.asana.com/api/1.0/projects/%lu/tasks";
  int length = (int)floor(log10((float)project->id)) + 1;

  char *url = malloc(strlen(project_tasks_path) + length + 1);
  sprintf(url, project_tasks_path, project->id);

  if(FETCH_OK == fetch(url, &json)) {
    project->tasks_head = parse_tasks(json);
  }
  if(json) {
    free(json);
  }
}

project_t* parse_projects(char *json) {
  json_t *root;
  json_error_t error;
  root = json_loads(json, 0, &error);

  project_t *head = NULL;
  json_t *data = json_object_get(root, "data");
  unsigned int projects_count = json_array_size(data);

  project_t *project_last = NULL;

  for(unsigned int i = 0; i < projects_count; i++ ) {
    project_t *project = NULL;
    if(NULL == head) {
      head = calloc(1, sizeof(project_t));
      project = head;
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
  return head;
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
  printf("%lu (%d): %s\n", project->id, project->whitelisted, project->name);
  
  task_t *task = project->tasks_head;
  while(task) {
    printf("- %lu: %s\n", task->id, task->name);
    task = task->next;
  }
}