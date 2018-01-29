#include <stdbool.h>

typedef struct {
  unsigned long int id;
  char *name;
  char *due_on;
  bool completed;  
} task_t;