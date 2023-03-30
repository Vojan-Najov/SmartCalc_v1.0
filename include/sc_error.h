#ifndef _SMARTCALC_ERROR_H
#define _SMARTCALC_ERROR_H

#include "sc_deque.h"

#define SC_BAD_ALLOC 1
#define SC_BAD_TOKEN 2
#define SC_BAD_EXPR 3
#define SC_BAD_BRACKET 4
#define SC_BAD_FUNCDEF 5
#define SC_BAD_VAR 6
#define SC_BAD_FUNC 7
#define SC_RECURSIVE_FUNC 8
#define SC_DEVIDE_BY_ZERO 9
#define SC_NO_TOKENS 10

#define SC_SUCCESS 0
#define SC_FAILURE 1

#define SC_HELP_MESSAGE \
	"Usage:\n" \
	"      SmartCalc_v1,0    -- with graphical user interface.\n" \
	"      SmartCalc_v1.0 -t -- with command line interface.\n" \
	"      SmartCalc_v1.0 -h -- print help message.\n"

#define SC_BAD_ALLOC_MESSAGE \
	"memory error: bad allocation"
#define SC_STDIN_ERROR_MESSAGE \
	"stdin: error"
#define SC_BAD_TOKEN_MESSAGE \
	"syntax error: bad token"
#define SC_BAD_EXPR_MESSAGE \
	"syntax error: invalid expression"
#define SC_BAD_BRACKETS_MESSAGE \
	"syntax error: unpaired brackets"
#define SC_BAD_FUNCDEF_MESSAGE \
	"syntax error: no parentheses found near the function"
#define SC_BAD_VAR_MESSAGE \
	"expression: variable not set"
#define SC_BAD_FUNC_MESSAGE \
	"expression: function not set"
#define SC_BAD_RECURSIVE_MESSAGE \
	"expression: recursive definitions are not supported"
#define SC_DEVIDE_BY_ZERO_MESSAGE \
	"calculation error: devide by zero"

/* Utilitary function, which print error message in stderr. */

int sc_print_error(const char *message);

/* Functions for handling errors of command line arguments. */

int sc_print_usage(void);

int sc_error_program_arg(const char *arg);

/* Functions for handling errors of input reading from terminal. */

void sc_error_input(void *line);

void sc_error_input_alloc(void);

/* Functions for handling lexer's errors. */

void sc_cli_error_lexer(int err_status, sc_deque_t *lexems);

char *sc_gui_error_lexer(int err_status, sc_deque_t *lexems);

/* Functions for handling scanner's errors. */

void sc_error_scanner(sc_deque_t *lexems);

void sc_cli_error_scanner(sc_deque_t *lexems);

char *sc_gui_error_scanner(sc_deque_t *lexems);


int sc_error_scanner_bad_expr(sc_deque_t *lexems);

void sc_error_parser_alloc(sc_deque_t *lexems,
                           sc_deque_t *rpn, sc_deque_t *stack);

int sc_error_parser_bad_bracket(sc_deque_t *lexems,
                                sc_deque_t *stack, sc_deque_t **rpn);

int sc_error_parser_bad_funcdef(sc_deque_t *lexems,
                                sc_deque_t *stack, sc_deque_t **rpn);

int sc_error_calc(sc_deque_t *stack, sc_deque_t *rpn, int error);

#endif
