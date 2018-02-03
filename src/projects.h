#ifndef PROJECTS_H
#define PROJECTS_H
#include <stdbool.h>
#include "./tasks.h"

typedef struct project {
  unsigned long int id;
  bool whitelisted;
  char *name;
  task_t *tasks_head;
  struct project *next;  
} project_t;

project_t* load_projects();
void load_project_tasks(project_t *project);
void print_project(project_t *project);
void whitelist_projects(project_t *project, unsigned long int *whitelist_ids);
#endif