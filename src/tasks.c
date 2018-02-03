#include <stdlib.h>
#include <jansson.h>
#include <string.h>
#include "./tasks.h"

void parse_task(task_t *task, json_t *data);

task_t *parse_tasks(char *json) {
  json_t *root;
  json_error_t error;
  root = json_loads(json, 0, &error);

  task_t *head = NULL;
  json_t *data = json_object_get(root, "data");
  unsigned int tasks_count = json_array_size(data);

  task_t *task_last = NULL;

  for(unsigned int i = 0; i < tasks_count; i++ ) {
    task_t *task = NULL;
    if(NULL == head) {
      head = calloc(1, sizeof(task_t));
      task = head;
    } else {
      task = calloc(1, sizeof(task_t));
    }
    json_t *task_data = json_array_get(data, i);
    parse_task(task, task_data);

    if(task_last) {
      task_last->next = task;
    }
    task_last = task;
  }

  json_decref(root);
  return head;
}

void parse_task(task_t *task, json_t *data) {
  json_t *id, *name;

  id = json_object_get(data, "id");
  json_int_t id_int = json_integer_value(id);
  task->id = id_int;

  name = json_object_get(data, "name");
  const char *name_str = json_string_value(name);

  task->name = malloc(strlen(name_str) + 1);
  strcpy(task->name, name_str);
}