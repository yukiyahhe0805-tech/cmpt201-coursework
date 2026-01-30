#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 5

int main(void) {
  char *hist[N] = {0};
  int next = 0, count = 0;

  char *line = NULL;
  size_t cap = 0;

  while (1) {
    printf("Enter input: ");
    fflush(stdout);

    ssize_t r = getline(&line, &cap, stdin);
    if (r == -1)
      break;

    free(hist[next]);

    hist[next] = malloc(strlen(line) + 1);
    if (!hist[next]) {
      perror("malloc");
      exit(1);
    }
    strcpy(hist[next], line);

    next = (next + 1) % N;
    if (count < N)
      count++;

    if (strcmp(line, "print\n") == 0 || strcmp(line, "print") == 0) {
      int start = (next - count + N) % N;
      for (int i = 0; i < count; i++) {
        int idx = (start + i) % N;
        fputs(hist[idx], stdout);
      }
    }
  }

  for (int i = 0; i < N; i++)
    free(hist[i]);
  free(line);
  return 0;
}
