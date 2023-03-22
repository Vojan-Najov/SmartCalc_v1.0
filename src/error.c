#include "error.h"
#include <stdio.h>
#include <stdlib.h>

static int print_error(char *message) {
	const char *programm_name = "SmartCalc_v1.0";

	fprintf(stderr, "%s: %s", programm_name, message);

	return (1);
}

int error_create_deque(void) {
	return (print_error("memory error: bad allocation"));
}

int error_deque_push(deque_t **deq) {
	(*deq)->clear(*deq);
	free(*deq);
	*deq = NULL;

	return (print_error("memory error: bad allocation"));
}

int error_bad_token(deque_t **deq) {
	(*deq)->clear(*deq);
	free(*deq);
	*deq = NULL;

	return (print_error("syntax error"));
}

int error_parser_alloc(deque_t *lexems, deque_t *stack, deque_t **rpn) {
	lexems->clear(lexems);
	free(lexems);
	if (stack != NULL) {
		stack->clear(stack);
	}
	free(stack);
	if (*rpn != NULL) {
		(*rpn)->clear(*rpn);
	}
	free(*rpn);
	*rpn = NULL;

	return (print_error("memory error: bad allocation"));
}

int error_parser_bad_bracket(deque_t *lexems, deque_t *stack, deque_t **rpn) {
	lexems->clear(lexems);
	free(lexems);
	if (stack != NULL) {
		stack->clear(stack);
	}
	free(stack);
	if (*rpn != NULL) {
		(*rpn)->clear(*rpn);
	}
	free(*rpn);
	*rpn = NULL;

	return (print_error("unpaired brackets"));
}

int error_calculate_alloc(deque_t *stack, deque_t *rpn) {
	if (stack != NULL) {
		stack->clear(stack);
		free(stack);
	}
	rpn->clear(rpn);
	free(rpn);

	return (print_error("memory error: bad allocation"));
}

int error_calculate_bad_token(deque_t *stack, deque_t *rpn) {
	stack->clear(stack);
	free(stack);
	rpn->clear(rpn);
	free(rpn);

	print_error("memory error: syntax error");
	return (SMARTCALC_BAD_TOKEN);
}
