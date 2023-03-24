#include "sc_error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sc_print_error(const char *message) {
	const char *programm_name = "SmartCalc_v1.0";

	fprintf(stderr, "%s: %s\n", programm_name, message);

	return (SC_FAILURE);
}

int sc_print_usage(void) {
	fprintf(stderr, SC_HELP_MESSAGE);

	return (SC_FAILURE);
}

int sc_error_program_arg(const char *arg) {
	char buf[128] = "unknown option: ";
	size_t n = 128 - sizeof("unknown option: ") - 1;

	strncat(buf, arg, n);
	sc_print_error(buf);
	sc_print_usage();

	return (SC_FAILURE);
}

void sc_error_input(void *line) {
	free(line);
	sc_print_error(SC_STDIN_ERROR_MESSAGE);
	exit(EXIT_FAILURE);
}

void sc_error_input_alloc(void) {
	sc_print_error(SC_BAD_ALLOC_MESSAGE);
	exit(EXIT_FAILURE);
}

void sc_error_lexer(deque_t *lexems) {
	if (lexems != NULL) {
		lexems->clear(lexems);
	}
	sc_print_error(SC_BAD_ALLOC_MESSAGE);
	exit(EXIT_FAILURE);
}

int sc_error_lexer_bad_token(deque_t **lexems) {
	if (*lexems != NULL) {
		(*lexems)->clear(*lexems);
		*lexems = NULL;
	}
	sc_print_error(SC_LEXER_ERROR_MESSAGE);

	return (SC_BAD_TOKEN);
}

void sc_error_scanner(deque_t *lexems) {
	if (lexems != NULL) {
		lexems->clear(lexems);
	}
	sc_print_error(SC_BAD_ALLOC_MESSAGE);
	exit(EXIT_FAILURE);
}

void sc_error_parser_alloc(deque_t *lexems, deque_t *rpn, deque_t *stack) {
	if (lexems != NULL) {
		lexems->clear(lexems);
	}
	if (rpn != NULL) {
		rpn->clear(rpn);
	}
	if (lexems != NULL) {
		stack->clear(stack);
	}
	sc_print_error(SC_BAD_ALLOC_MESSAGE);
	exit(EXIT_FAILURE);
}

int sc_error_parser_bad_bracket(deque_t *lexems, deque_t *stack, deque_t **rpn) {
	if (lexems != NULL) {
		lexems->clear(lexems);
	}
	if (stack != NULL) {
		stack->clear(stack);
	}
	if (*rpn != NULL) {
		(*rpn)->clear(*rpn);
		*rpn = NULL;
	}

	sc_print_error(SC_BAD_BRACKETS_MESSAGE);

	return (SC_BAD_BRACKET);
}

int sc_error_parser_bad_funcdef(deque_t *lexems, deque_t *stack, deque_t **rpn) {
	if (lexems != NULL) {
		lexems->clear(lexems);
	}
	if (stack != NULL) {
		stack->clear(stack);
	}
	if (*rpn != NULL) {
		(*rpn)->clear(*rpn);
		*rpn = NULL;
	}

	sc_print_error(SC_BAD_FUNCDEF_MESSAGE);

	return (SC_BAD_FUNCDEF);
}

void sc_error_calc_alloc(deque_t *stack, deque_t *rpn) {
	if (stack != NULL) {
		stack->clear(stack);
	}
	if (rpn != NULL) {
		rpn->clear(rpn);
	}
	sc_print_error(SC_BAD_ALLOC_MESSAGE);
	exit(EXIT_FAILURE);
}

int sc_error_calc_devide_by_zero(deque_t *stack, deque_t *rpn) {
	if (stack != NULL) {
		stack->clear(stack);
	}
	if (rpn != NULL) {
		rpn->clear(rpn);
	}
	sc_print_error(SC_DEVIDE_BY_ZERO_MESSAGE);

	return (SC_DEVIDE_BY_ZERO);
}

int sc_error_calc_bad_token(deque_t *stack, deque_t *rpn) {
	stack->clear(stack);
	rpn->clear(rpn);
	sc_print_error(SC_CALC_ERROR_MESSAGE);

	return (SC_BAD_TOKEN);
}

int sc_error_calc_bad_var(deque_t *stack, deque_t *rpn) {
	stack->clear(stack);
	rpn->clear(rpn);
	sc_print_error(SC_CALC_BAD_VAR_MESSAGE);

	return (SC_BAD_TOKEN);
}
