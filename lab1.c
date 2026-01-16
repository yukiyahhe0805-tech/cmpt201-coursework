
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  char *line = NULL;
  size_t cap = 0;

  while (1) {
    printf("Please enter some text: ");
    fflush(stdout);

    ssize_t len = getline(&line, &cap, stdin);
    if (len == -1) {
      break;
    }

    if (len > 0 && line[len - 1] == '\n') {
      line[len - 1] = '\0';
    }

    printf("Tokens:\n");

    char *saveptr;
    char *tok = strtok_r(line, " ", &saveptr);
    while (tok != NULL) {
      printf("  %s\n", tok);
      tok = strtok_r(NULL, " ", &saveptr);
    }
  }

  free(line);
  return 0;
}
