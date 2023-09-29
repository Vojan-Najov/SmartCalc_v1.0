#include "sc_input.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "sc_error.h"

int sc_input_term(char **str) {
  static size_t size = 256;
  static char *line = NULL;
  char c;
  size_t n = 0;
  int status = SC_INPUT_OK;

  if (line == NULL) {
    line = (char *)malloc(sizeof(char) * size);
  }
  if (isatty(STDOUT_FILENO)) {
    printf("> ");
  }

  while ((c = fgetc(stdin)) != EOF) {
    if (n >= size - 1) {
      size *= 2;
      line = realloc(line, size);
    }
    if (line == NULL || c == '\n') {
      break;
    } else {
      line[n] = c;
      ++n;
    }
  }
  if (line == NULL) {
    sc_error_input_alloc();
  } else if (feof(stdin) && n == 0) {
    fprintf(stdout, "exit\n");
    status = SC_INPUT_FAIL;
  } else if (!strncmp(line, "exit", 4)) {
    status = SC_INPUT_FAIL;
  } else if (ferror(stdin)) {
    sc_error_input(line);
  } else {
    line[n] = '\0';
  }

  *str = line;

  return (status);
}
