#include "sc_error.h"
#include "sc_input.h"
#include "sc_lexer.h"

#include "sc_scanner.h"
#include "sc_variable.h"
#include "sc_calculator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sc_parser.h"

static int smartcalc_gui(void);
static int smartcalc_cli(void);

int main(int argc, char **argv) {
	int exit_status;

	if (argc == 1) {
		exit_status = smartcalc_gui();
	} else if (strcmp(argv[1], "-t") == 0) {
		exit_status = smartcalc_cli();
	} else if (strcmp(argv[1], "-h") == 0) {
		sc_print_usage();
		exit_status = SC_SUCCESS;
	} else {
		sc_error_program_arg(argv[1]);
		exit_status = SC_FAILURE;
	}

	return (exit_status);
}

static int smartcalc_gui(void) {
	fprintf(stderr, "Sorry, gui not supported\n");
	return (0);
}

static int smartcalc_cli(void) {
	deque_t *lexems;
	int status = 0;
	char *str = NULL;

	while(sc_input_term(&str)) {
		lexems = sc_lexer(str);
		if (lexems == NULL) {
			continue;
		}
	}

	free(str);

	return(status);
}
/*
}
	double var;
	int expr_type;
	int err_status;
	int (*input)(char **);

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
			/ VARIABLE ASSIGNMENT /
			err_status = sc_calculate(rpn, &var);
			if (!err_status) {
				sc_set_variable(var);
				printf("x = %f\n\n", var); 
			}
		} else if (expr_type == SC_EXPRESSION) {
			/ EXPRESSION CALCULATION /
			err_status = sc_calculate(rpn, &var);
			if (!err_status) {
				printf("result = %f\n", var);
			}
		} else if (expr_type == SC_DEFINITION) {
			/ FUNCTION DEFINITION /
			err_status = sc_definition(rpn);
		}
		printf("\n");
		printf(":> ");
	}
*/


