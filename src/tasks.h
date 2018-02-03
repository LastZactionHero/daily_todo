#ifndef TASKS_H
#define TASKS_H
#include <stdbool.h>

typedef struct {
  unsigned long int id;
  char *name;
  char *due_on;
  bool completed;  
} task_t;

task_t *parse_tasks(char *json);
#endif