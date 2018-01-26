#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./projects.h"
#include "./fetch.h"

fetch_result_t fetch_projects(char **json);

void load_projects() {
  printf("Fetching Projects...\n");

  char *json = NULL;
  if(FETCH_OK == fetch("https://app.asana.com/api/1.0/projects", &json)) {
    printf("%s\n", json);
  }
  if(json) {
    free(json);
  }
}