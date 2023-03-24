#include "input.h"
#include "sc_error.h"
#include <stdlib.h>
#include <stdio.h>

int sc_input_term(char **str) {
	static size_t size = 256;
	static char *line = NULL;
	int status = SC_INPUT_OK;
	char c = '\0';
	size_t n = 0;

	if (line == NULL) {
		line = (char *) malloc(sizeof(char) * size);
	}
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
	} else if (feof(stdin) && n == 0) {
		status = SC_INPUT_FAIL;
	} else if (ferror(stdin)) {
		sc_error_input(line);
	} else {
		line[n] = '\0'; 
	}

	*str = line;

	return (status);
}
