#define _DEFAULT_SOURCE
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define EXTRA_SIZE 256
#define BLOCK_SIZE 128
#define BUF_SIZE 256

struct header {
  uint64_t size;
  struct header *next;
};

static void handle_error(const char *msg) {
  char buf[BUF_SIZE];
  int len = snprintf(buf, BUF_SIZE, "ERROR: %s: %s\n", msg, strerror(errno));
  if (len > 0) {
    write(STDERR_FILENO, buf, (size_t)len);
  }
  exit(1);
}

static void print_out(char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE];
  ssize_t len;

  if (data_size == sizeof(uint64_t)) {
    len = snprintf(buf, BUF_SIZE, format, *(uint64_t *)data);
  } else {
    len = snprintf(buf, BUF_SIZE, format, *(void **)data);
  }

  if (len < 0)
    handle_error("snprintf");
  if (write(STDOUT_FILENO, buf, (size_t)len) < 0)
    handle_error("write");
}

int main(void) {
  errno = 0;
  void *base = sbrk(EXTRA_SIZE);
  if (base == (void *)-1) {
    handle_error("sbrk");
  }

  void *block1 = base;
  void *block2 = (void *)((char *)base + BLOCK_SIZE);

  struct header *h1 = (struct header *)block1;
  struct header *h2 = (struct header *)block2;

  h1->size = BLOCK_SIZE;
  h1->next = NULL;

  h2->size = BLOCK_SIZE;
  h2->next = h1;

  size_t header_size = sizeof(struct header);
  size_t data_size = BLOCK_SIZE - header_size;

  unsigned char *data1 = (unsigned char *)block1 + header_size;
  unsigned char *data2 = (unsigned char *)block2 + header_size;

  for (size_t i = 0; i < data_size; i++) {
    data1[i] = 0;
    data2[i] = 1;
  }

  print_out("first block:       %p\n", &block1, sizeof(&block1));
  print_out("second block:      %p\n", &block2, sizeof(&block2));

  print_out("first block size:  %lu\n", &h1->size, sizeof(uint64_t));
  print_out("first block next:  %p\n", &h1->next, sizeof(&h1->next));

  print_out("second block size: %lu\n", &h2->size, sizeof(uint64_t));
  print_out("second block next: %p\n", &h2->next, sizeof(&h2->next));

  for (size_t i = 0; i < data_size; i++) {
    uint64_t v = data1[i];
    print_out("%lu\n", &v, sizeof(uint64_t));
  }

  for (size_t i = 0; i < data_size; i++) {
    uint64_t v = data2[i];
    print_out("%lu\n", &v, sizeof(uint64_t));
  }

  return 0;
}
