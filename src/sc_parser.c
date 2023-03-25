#include "sc_parser.h"
#include "sc_error.h"
#include <stddef.h>

static int _shunting_yard_algorithm(sc_deque_t *lexems,
                                    sc_deque_t *rpn, sc_deque_t *stack);

static int _check_priority(sc_token_t *token1, sc_token_t *token2);

sc_deque_t *parser(sc_deque_t *lexems) {
	sc_deque_t *rpn;
	sc_deque_t *stack;
	int err_status = 0;

	rpn = sc_create_deque();
	stack = sc_create_deque();
	if (rpn == NULL || stack == NULL) {
		sc_error_parser_alloc(lexems, rpn, stack);
	}

	err_status = _shunting_yard_algorithm(lexems, rpn, stack);
	if (err_status == SC_BAD_ALLOC) {
		sc_error_parser_alloc(lexems, rpn, stack);
	} else if (err_status == SC_BAD_BRACKET) {
		sc_error_parser_bad_bracket(lexems, stack, &rpn);
	} else if (err_status == SC_BAD_FUNCDEF) {
		sc_error_parser_bad_funcdef(lexems, stack, &rpn);
	} else {
		lexems->clear(lexems);
		stack->clear(stack);
	}

	return (rpn);
}

static int _shunting_yard_algorithm(sc_deque_t *lexems,
									sc_deque_t *rpn, sc_deque_t *stack) {
	int err_status = 0;
	sc_token_t token;

	while(!err_status && !lexems->is_empty(lexems)) {
		token = lexems->pop_front(lexems);
		if (token.type == SC_NUMBER || token.type == SC_VAR) {
			err_status = rpn->push_back(rpn, &token);
		} else if (token.type == SC_FUNCTION) {
			if (!lexems->is_empty(lexems) && \
               lexems->peek_front(lexems)->type == SC_LBRACKET) {
				err_status = stack->push_front(stack, &token);
			} else {
				err_status = SC_BAD_FUNCDEF;
			}
		} else if (token.type == SC_UNARY_OP || token.type == SC_LBRACKET) {
			err_status = stack->push_front(stack, &token);
		} else if (token.type == SC_BINARY_OP) {
			while (!err_status && !stack->is_empty(stack) && \
                  _check_priority(&token, stack->peek_front(stack))) {
				sc_token_t tmp = stack->pop_front(stack);
				err_status = rpn->push_back(rpn, &tmp);
			}
			err_status = stack->push_front(stack, &token);
		} else if (token.type == SC_RBRACKET) {
			while (!err_status && !stack->is_empty(stack) && \
                  stack->peek_front(stack)->type != SC_LBRACKET) {
				token = stack->pop_front(stack);
				err_status = rpn->push_back(rpn, &token);
			}
			if (!err_status && stack->is_empty(stack)) {
				err_status = SC_BAD_BRACKET;
			} else if (!err_status) {
				/* There is LBRACKET in the top of the stack */
				stack->pop_front(stack);
				if (!stack->is_empty(stack) && \
                   (stack->peek_front(stack)->type == SC_UNARY_OP ||
                   stack->peek_front(stack)->type == SC_FUNCTION)) {
					token = stack->pop_front(stack);
					err_status = rpn->push_back(rpn, &token);
				}
			}
		}
	}
	while(!err_status && !stack->is_empty(stack)) {
		token = stack->pop_front(stack);
		if (token.type == SC_LBRACKET) {
			err_status = SC_BAD_BRACKET;
		} else {
			err_status = rpn->push_back(rpn, &token);
		}
	}

	return (err_status);
}

static int _check_priority(sc_token_t *token1, sc_token_t *token2) {
	int o1, o2;
	enum sc_associativity {SC_LEFT, SC_RIGHT};
	static int priority[] = {
		2, // ADD
		2, // SUB
		3, // MUL
		3, // DIV
		3, // MOD
		4, // POW
	};
	static int associativity[] = {
		SC_LEFT, // ADD
		SC_LEFT, // SUB
		SC_LEFT, // MUL
		SC_LEFT, // DIV
		SC_LEFT, // MOD
		SC_RIGHT, // POW
	};
	int ret = 0;

	if (token2->type == SC_BINARY_OP) {
		o1 = token1->value.binary_op;
		o2 = token2->value.binary_op;
		ret = priority[o2] > priority[o1] || \
			  (priority[o2] == priority[o1] && associativity[o1] == SC_LEFT);
	} else if (token2->type == SC_UNARY_OP || token2->type == SC_FUNCTION) {
		ret = 1;
	}

	return (ret);
}
