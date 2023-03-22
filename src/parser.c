#include "parser.h"
#include "error.h"
#include <stdlib.h>
#include <stdio.h>

int check_priority(token_t *token1, token_t *token2) {
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

deque_t *parser(deque_t *lexems) {
	deque_t *rpn;
	deque_t *stack;
	token_t token;
	int err_status = 0;

	rpn = create_deque();
	stack = create_deque();
	if (rpn == NULL || stack == NULL) {
		error_parser_alloc(lexems, stack, &rpn);
	} else {
		while(!err_status && !lexems->is_empty(lexems)) {
			token = lexems->pop_front(lexems);
			if (token.type == NUMBER || token.type == VAR) {
				err_status = rpn->push_back(rpn, &token);
			} else if (token.type == FUNCTION || \
					   token.type == UNARY_OP || token.type == LBRACKET) {
				err_status = stack->push_front(stack, &token);
			} else if (token.type == BINARY_OP) {
				while (!err_status && !stack->is_empty(stack) && \
					   check_priority(&token, stack->peek_front(stack))) {
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
				if (!err_status && stack->peek_front(stack)->type == LBRACKET) {
					stack->pop_front(stack);
					if (!stack->is_empty(stack) && \
						(stack->peek_front(stack)->type == UNARY_OP ||
						 stack->peek_front(stack)->type == FUNCTION)) {
						token = stack->pop_front(stack);
						err_status = rpn->push_back(rpn, &token);
					}
				} else if (!err_status) {
					err_status = error_parser_bad_bracket(lexems, stack, &rpn);
				}
			}
		}
		while(!err_status && !stack->is_empty(stack)) {
			token = stack->pop_front(stack);
			if (token.type == LBRACKET) {
				err_status = error_parser_bad_bracket(lexems, stack, &rpn);
			} else {
				err_status = rpn->push_back(rpn, &token);
			}
		}
		if (err_status) {
			error_parser_alloc(lexems, stack, &rpn);
		} else {
			free(stack);
			free(lexems);
		}
	}

	return (rpn);
}
