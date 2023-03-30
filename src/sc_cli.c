#include "sc_input.h"
#include "sc_lexer.h"
#include "sc_scanner.h"
#include "sc_parser.h"
#include "sc_calculator.h"
#include "sc_variable.h"
#include "sc_error.h"

#include <stdlib.h>
#include <stdio.h>

int smartcalc_cli(void) {
	sc_deque_t lexems, *rpn;
	int expr_type;
	double result;
	int err_status = 0;
	char *str = NULL;

	while(sc_input_term(&str)) {
		sc_init_deque(&lexems);
		err_status = sc_lexer(str, &lexems);
		if (err_status) {
			sc_cli_error_lexer(err_status, &lexems);
			continue;
		}
		expr_type = sc_scanner(&lexems);
		if (expr_type == SC_SCANNER_ERROR) {
			sc_cli_error_scanner(&lexems);
			continue;
		}


		rpn = parser(&lexems);
		//lexems.clear(&lexems);
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

	return(err_status);
}
