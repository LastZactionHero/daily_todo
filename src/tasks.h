#ifndef TASKS_H
#define TASKS_H
#include <stdbool.h>

typedef struct task {
  unsigned long int id;
  char *name;
  char *due_on;
  bool completed;
  struct task *next;
} task_t;

task_t *parse_tasks(char *json);
#endif