#include "sc_error.h"
#include "sc_input.h"
#include "sc_lexer.h"
#include "sc_scanner.h"
#include "sc_parser.h"
#include "sc_calculator.h"
#include "sc_variable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	fprintf(stderr, "Sorry, gui not supported now.\n");
	return (0);
}

static int smartcalc_cli(void) {
	sc_deque_t *lexems, *rpn;
	int expr_type;
	double result;
	int status = 0;
	char *str = NULL;

	while(sc_input_term(&str)) {
		lexems = sc_lexer(str);
		if (lexems == NULL) {
			continue;
		}
		expr_type = sc_scanner(lexems);
		if (expr_type == SC_BAD_EXPR) {
			continue;
		}
		rpn = parser(lexems);
		if (rpn == NULL) {
			continue;
		}
		if (expr_type == SC_ASSIGNMENT) {
			sc_assignment(rpn);
		} else if (expr_type == SC_DEFINITION) {
			sc_definition(rpn);
		} else if (expr_type == SC_EXPRESSION) {
			if (sc_calculation(rpn, &result) == 0) {
				printf("%.7f\n", result);
			}
		}
	}

	free(str);

	return(status);
}
