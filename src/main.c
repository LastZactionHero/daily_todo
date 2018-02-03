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
  projects_head = load_projects();

  unsigned long int *project_id_whitelist;
  load_project_whitelist(&project_id_whitelist);  
  whitelist_projects(projects_head, project_id_whitelist);
  free(project_id_whitelist);

  project_t *project = projects_head;
  while(project) {        
    if(project->whitelisted) {
      load_project_tasks(project);
      print_project(project);      
    }    
    project = project->next;
  }

  // TODO: Free projects

  return 0;
}