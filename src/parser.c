#include "parser.h"
#include <stdlib.h>
#include <stdio.h>

int PRIORITY_CORRECT(int o1, int o2) {
	static int priority[] = {
		1, // ADD
		1, // SUB
		2, // MULT
		2, // DIV
		2, // MOD
		3, // POW
	};

	printf("prior = %d\n",o1);
	printf("prior = %d\n",o2);
	printf("prior = %d\n",priority[o1]);
	printf("prior = %d\n",priority[o2]);
	printf("prior = %d\n",(priority[o2] >= priority[o1]));
	return (priority[o2] >= priority[o1]);
}

deque_t *parser(deque_t *lexems) {
	deque_t *rpn;
	deque_t *stack;
	data_t token;
	int status;

	init_deque(&rpn);
	if (rpn == NULL) {
		lexems->clear(lexems);
		free(lexems);
		perror("SmartCalc");
		exit(EXIT_FAILURE);
	}
	init_deque(&stack);
	if (rpn == NULL) {
		lexems->clear(lexems);
		free(lexems);
		perror("SmartCalc");
		exit(EXIT_FAILURE);
	}
	while (!lexems->is_empty(lexems)) {
		token = lexems->pop_front(lexems);
		if (token.type == OPERAND || token.type == VAR) {
			status = rpn->push_back(rpn, &token);
			if (status) {
				exit(EXIT_FAILURE);
			}
		} else if (token.type == UNARY_OP || token.type == LBRACKET) {
			status = stack->push_front(stack, &token);
			if (status) {
				exit(EXIT_FAILURE);
			}
		} else if (token.type == BINARY_OP) {
			while (!stack->is_empty(stack) && \
				   stack->peek_front(stack)->type != LBRACKET && \
				   PRIORITY_CORRECT(token.content.binary_op, stack->peek_front(stack)->content.binary_op)) {
				data_t tmp = stack->pop_front(stack);
				status = rpn->push_back(rpn, &tmp);
				if (status) {
					exit(EXIT_FAILURE);
				}
			}
			status = stack->push_front(stack, &token);
			if (status) {
				exit(EXIT_FAILURE);
			}
		} else if (token.type == RBRACKET) {
			while (!stack->is_empty(stack) && stack->peek_front(stack)->type != LBRACKET) {
				token = stack->pop_front(stack);
				status = rpn->push_back(rpn, &token);
				if (status) {
					exit(EXIT_FAILURE);
				}
			}
			if (stack->is_empty(stack)) {
				stack->clear(stack);
				lexems->clear(lexems);
				free(lexems); free(stack);
				return (NULL);
			} else {
				stack->pop_front(stack);
				if (!stack->is_empty(stack) && \
					stack->peek_front(stack)->type == UNARY_OP) {
					token = stack->pop_front(stack);
					status = rpn->push_back(rpn, &token);
					if (status) {
						exit(EXIT_FAILURE);
					}
				}
			}
		}
	}
	while (!stack->is_empty(stack)) {
		token = stack->pop_front(stack);
		if (token.type == LBRACKET) {
			stack->clear(stack);
			lexems->clear(lexems);
			free(lexems); free(stack);
			return (NULL);
		}
		status = rpn->push_back(rpn, &token);
		if (status) {
			exit(EXIT_FAILURE);
		}
	}

	free(stack);
	free(lexems);

	return (rpn);
}
