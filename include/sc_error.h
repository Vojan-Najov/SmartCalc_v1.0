#ifndef _SMARTCALC_ERROR_H
#define _SMARTCALC_ERROR_H

#include "sc_deque.h"

#define SC_BAD_ALLOC 1
#define SC_BAD_TOKEN 2
#define SC_BAD_BRACKET 3
#define SC_BAD_FUNCDEF 4
#define SC_BAD_VAR 5
#define SC_BAD_FUNC 6
#define SC_RECURSIVE_FUNC 7
#define SC_DEVIDE_BY_ZERO 8

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
#define SC_LEXER_ERROR_MESSAGE \
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

int sc_print_error(const char *message);

int sc_print_usage(void);

int sc_error_program_arg(const char *arg);

void sc_error_input(void *line);

void sc_error_input_alloc(void);

void sc_error_lexer(sc_deque_t *lexems);

int sc_error_lexer_bad_token(sc_deque_t **lexems);

void sc_error_scanner(sc_deque_t *lexems);

int sc_error_scanner_bad_expr(sc_deque_t *lexems);

void sc_error_parser_alloc(sc_deque_t *lexems,
                           sc_deque_t *rpn, sc_deque_t *stack);

int sc_error_parser_bad_bracket(sc_deque_t *lexems,
                                sc_deque_t *stack, sc_deque_t **rpn);

int sc_error_parser_bad_funcdef(sc_deque_t *lexems,
                                sc_deque_t *stack, sc_deque_t **rpn);

int sc_error_calc(sc_deque_t *stack, sc_deque_t *rpn, int error);

#endif
