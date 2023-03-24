#include "sc_error.h"
#include "sc_scanner.h"
#include "sc_lexer.h"
#include "sc_variable.h"
#include "sc_calculator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deque.h"
#include "input.h"
#include "parser.h"

int main(int argc, char **argv) {
	deque_t *lexems;
	deque_t *rpn;
	double var;
	int expr_type;
	int err_status;
	char *str = NULL;
	int (*input)(char **);

	if (argc == 1) {
		// input = &sc_input_gui;
		fprintf(stderr, "Sorry, gui not supported\n");
		return (0);
	} else if (strcmp(argv[1], "-t") == 0) {
		input = &sc_input_term;		
	} else if (strcmp(argv[1], "-h") == 0) {
		sc_print_usage();
		return (SC_SUCCESS);
	} else {
		sc_error_program_arg(argv[1]);
		return (SC_FAILURE);
	}

	while (input(&str)) {
		lexems = sc_lexer(str);
		if (lexems == NULL) {
			continue;
		}
		printf("    lexems:\n");
		deque_print(lexems);

		expr_type = sc_scanner(lexems);
		printf("    scanner:\n");
		deque_print(lexems);
		rpn = parser(lexems);
		if (rpn == NULL) {
			continue;
		}
		printf("    parser:\n");
		deque_print(rpn);

		if (expr_type == SC_ASSIGNMENT) {
			/* VARIABLE ASSIGNMENT */
			err_status = sc_calculate(rpn, &var);
			if (!err_status) {
				sc_set_variable(var);
				printf("x = %f\n\n", var); 
			}
		} else if (expr_type == SC_EXPRESSION) {
			/* EXPRESSION CALCULATION */
			err_status = sc_calculate(rpn, &var);
			if (!err_status) {
				printf("result = %f\n", var);
			}
		} else if (expr_type == SC_DEFINITION) {
			/* FUNCTION DEFINITION */
			err_status = sc_definition(rpn);
		}
		printf("\n");
	}

	free(str);

	return (SC_SUCCESS);
}
