#include "sc_input.h"
#include "sc_error.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int sc_input_term(char **str) {
	static size_t size = 256;
	static char *line = NULL;
	char c;
	size_t n = 0;
	int status = SC_INPUT_OK;

	if (line == NULL) {
		line = (char *) malloc(sizeof(char) * size);
	}
	printf("> ");
	while (42) {
		c = fgetc(stdin);
		if (n >= size - 1) {
			size *= 2;
			line = realloc(line, size);
		}
		if (line == NULL || c == '\n' || c == EOF) {
			break;
		} else {
			line[n] = c;
			++n;
		}
	}
	if (line == NULL) {
		sc_error_input_alloc();
	} else if (ferror(stdin)) {
		sc_error_input(line);
	} else if (feof(stdin) && n == 0) {
		printf("exit\n");
		status = SC_INPUT_FAIL;
	} else if (!strcmp(line, "exit")) {
		status = SC_INPUT_FAIL;
	} else {
		line[n] = '\0'; 
	}

	*str = line;

	return (status);
}