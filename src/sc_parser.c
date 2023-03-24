#include "sc_parser.h"
#include "sc_error.h"
#include <stdlib.h>
#include <stdio.h>

static int _shunting_yard_algorithm(deque_t *lexems, deque_t *rpn, deque_t *stack);
static int _check_priority(token_t *token1, token_t *token2);

deque_t *parser(deque_t *lexems) {
	deque_t *rpn;
	deque_t *stack;
	int err_status = 0;

	rpn = create_deque();
	stack = create_deque();
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
		if (rpn->is_empty(rpn)) {
			// syntax error
			rpn->clear(rpn);
			rpn = NULL;
		}
		lexems->clear(lexems);
		stack->clear(stack);
	}

	return (rpn);
}

static int _shunting_yard_algorithm(deque_t *lexems, deque_t *rpn, deque_t *stack) {
	int err_status = 0;
	token_t token;

	while(!err_status && !lexems->is_empty(lexems)) {
		token = lexems->pop_front(lexems);
		if (token.type == NUMBER || token.type == VAR) {
			err_status = rpn->push_back(rpn, &token);
		} else if (token.type == FUNCTION) {
			if (!lexems->is_empty(lexems) && \
			     lexems->peek_front(lexems)->type == LBRACKET) {
				err_status = stack->push_front(stack, &token);
			} else {
				err_status = SC_BAD_FUNCDEF;
			}
		} else if (token.type == UNARY_OP || token.type == LBRACKET) {
			err_status = stack->push_front(stack, &token);
		} else if (token.type == BINARY_OP) {
			while (!err_status && !stack->is_empty(stack) && \
				   _check_priority(&token, stack->peek_front(stack))) {
				token_t tmp = stack->pop_front(stack);
				err_status = rpn->push_back(rpn, &tmp);
			}
			err_status = stack->push_front(stack, &token);
		} else if (token.type == RBRACKET) {
			while (!err_status && !stack->is_empty(stack) && \
				   stack->peek_front(stack)->type != LBRACKET) {
				token = stack->pop_front(stack);
				err_status = rpn->push_back(rpn, &token);
			}
			if (!err_status && stack->is_empty(stack)) {
				err_status = SC_BAD_BRACKET;
			} else if (!err_status) {
				/* There is LBRACKET in the top of the stack */
				stack->pop_front(stack);
				if (!stack->is_empty(stack) && \
					 (stack->peek_front(stack)->type == UNARY_OP ||
					 stack->peek_front(stack)->type == FUNCTION)) {
					token = stack->pop_front(stack);
					err_status = rpn->push_back(rpn, &token);
				}
			}
		}
	}
	while(!err_status && !stack->is_empty(stack)) {
		token = stack->pop_front(stack);
		if (token.type == LBRACKET) {
			err_status = SC_BAD_BRACKET;
		} else {
			err_status = rpn->push_back(rpn, &token);
		}
	}

	return (err_status);
}

static int _check_priority(token_t *token1, token_t *token2) {
	int o1, o2;
	enum ASSOCIATIVITY {LEFT, RIGHT};
	static int priority[] = {
		2, // ADD
		2, // SUB
		3, // MUL
		3, // DIV
		3, // MOD
		4, // POW
	};
	static int associativity[] = {
		LEFT, // ADD
		LEFT, // SUB
		LEFT, // MUL
		LEFT, // DIV
		LEFT, // MOD
		RIGHT, // POW
	};
	int ret = 0;

	if (token2->type == BINARY_OP) {
		o1 = token1->value.binary_op;
		o2 = token2->value.binary_op;
		ret = priority[o2] > priority[o1] || \
			  (priority[o2] == priority[o1] && associativity[o1] == LEFT);
	} else if (token2->type == UNARY_OP || token2->type == FUNCTION) {
		ret = 1;
	}

	return (ret);
}
