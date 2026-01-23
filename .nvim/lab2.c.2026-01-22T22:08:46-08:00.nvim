#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  char input[4096];

  while (1) {
    printf("Enter programs to run.\n> ");
    fflush(stdout);

    if (fgets(input, sizeof(input), stdin) == NULL) {
      break;
    }

    input[strcspn(input, "\n")] = '\0';

    char *saveptr;
    char *path = strtok_r(input, " ", &saveptr);

    if (path == NULL) {
      continue;
    }

    pid_t pid = fork();

    if (pid == 0) {
      execl(path, path, (char *)NULL);
      printf("Exec failure\n");
      exit(1);
    } else {
      waitpid(pid, NULL, 0);
    }
  }

  return 0;
}
