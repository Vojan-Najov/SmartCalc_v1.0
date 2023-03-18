#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deque.h"
#include "lexer.h"

int main() {
	ssize_t nread;
	size_t n = 256;
	char *str, *ptr;

	str = (char *) malloc(sizeof(char) * n);
	if (str == NULL) {
		perror("SmartCalc");
		exit(EXIT_FAILURE);
	}

	deque_t *deq;
	init_deque(&deq);
	for (int i = 0; i < 10; ++i) {
		data_t d = {0};
		deq->push_back(deq, &d);
	}
	for (int i = 0; i < 10; ++i) {
		data_t d = {0};
		deq->push_front(deq, &d);
	}
	for (int i = 0; i < 5; ++i) {
		data_t d = deq->pop_front(deq);
		(void) d;
	}
	for (int i = 0; i < 5; ++i) {
		data_t d = deq->pop_back(deq);
		(void) d;
	}
	deq->clear(deq);

	free(deq);

	do {
		nread = getline(&str, &n, stdin);
		ptr = strchr(str, '\n');
		if (ptr != NULL) {
			*ptr = '\0';
			fprintf(stdout, "%s\n", str);
			deque_t *lexems = lexer(str);
			if (lexems) {
				deque_print(lexems);
				deque_clear(lexems);
				free(lexems);
			}
		}
	} while (nread > 0);

	free(str);

	return (0);
}
