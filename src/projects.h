typedef struct project {
  unsigned long int id;
  char *name;
  struct project *next;
} project_t;

void load_projects(project_t **head);
void print_project(project_t *project);