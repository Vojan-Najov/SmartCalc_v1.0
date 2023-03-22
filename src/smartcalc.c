#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deque.h"
#include "lexer.h"
#include "parser.h"

int main() {
	ssize_t nread;
	size_t n = 256;
	char *str, *ptr;

	str = (char *) malloc(sizeof(char) * n);
	if (str == NULL) {
		perror("SmartCalc");
		exit(EXIT_FAILURE);
	}

	nread = getline(&str, &n, stdin);
	while (nread > 0) {
		ptr = strchr(str, '\n');
		if (ptr != NULL) {
			*ptr = '\0';
			deque_t *lexems = lexer(str);
			if (lexems) {
				printf("lexems\n");
				deque_print(lexems);
				deque_t *rpn = parser(lexems);
				printf("rpn\n");
				deque_print(rpn);
				deque_clear(rpn);
				free(rpn);
			} else {
				printf("lexems error\n");
			}
		}
		nread = getline(&str, &n, stdin);
	}

	free(str);

	return (0);
}
