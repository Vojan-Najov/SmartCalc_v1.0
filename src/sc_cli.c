#include <stdio.h>
#include <stdlib.h>

#include "sc_calculator.h"
#include "sc_error.h"
#include "sc_input.h"
#include "sc_lexer.h"
#include "sc_parser.h"
#include "sc_scanner.h"
#include "sc_variable.h"

int smartcalc_cli(void) {
  sc_deque_t lexems, rpn;
  int expr_type;
  double result;
  int err_status = 0;
  char *str = NULL;

  sc_init_deque(&lexems);
  sc_init_deque(&rpn);
  while (sc_input_term(&str)) {
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
    err_status = sc_parser(&lexems, &rpn);
    lexems.clear(&lexems);
    if (err_status) {
      sc_cli_error_parser(err_status, &lexems, &rpn);
      continue;
    }
    if (expr_type == SC_ASSIGNMENT) {
      err_status = sc_assignment(&rpn);
    } else if (expr_type == SC_DEFINITION) {
      err_status = sc_definition(&rpn);
    } else if (expr_type == SC_EXPRESSION) {
      err_status = sc_calculation(&rpn, &result);
      if (!err_status) {
        fprintf(stdout, "%.16g\n", result);
      }
    }
    if (err_status) {
      sc_cli_error_calculator(err_status, &rpn);
    } else {
      rpn.clear(&rpn);
    }
  }

  free(str);

  return (err_status);
}
