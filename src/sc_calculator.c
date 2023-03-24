#include "sc_calculator.h"
#include "sc_error.h"
#include "sc_variable.h"
#include <math.h>
#include <stdlib.h>

static void sc_minus(token_t *num);
static void sc_plus(token_t *num);

static void sc_cos(token_t *num);
static void sc_sin(token_t *num);
static void sc_tan(token_t *num);
static void sc_acos(token_t *num);
static void sc_asin(token_t *num);
static void sc_atan(token_t *num);
static void sc_sqrt(token_t *num);
static void sc_ln(token_t *num);
static void sc_log(token_t *num);
static void sc_f(token_t *num);

static void sc_add(token_t *num1, token_t *num2, token_t *result);
static void sc_sub(token_t *num1, token_t *num2, token_t *result);
static void sc_mul(token_t *num1, token_t *num2, token_t *result);
static void sc_div(token_t *num1, token_t *num2, token_t *result);
static void sc_mod(token_t *num1, token_t *num2, token_t *result);
static void sc_pow(token_t *num1, token_t *num2, token_t *result);

static void (*unary_op[2])(token_t *) = {
	&sc_minus,
	&sc_plus,
};

static void (*function[])(token_t *) = {
	&sc_cos,
	&sc_sin,
	&sc_tan,
	&sc_acos,
	&sc_asin,
	&sc_atan,
	&sc_sqrt,
	&sc_ln,
	&sc_log,
	&sc_f,
};

static void (*binary_op[])(token_t *, token_t *, token_t *) = {
	&sc_add,
	&sc_sub,
	&sc_mul,
	&sc_div,
	&sc_mod,
	&sc_pow,
};

int sc_calculate(deque_t *rpn, double *result) {
	deque_t *stack;
	token_t token;
	int err_status = 0;
	double var;

	stack = create_deque();
	if (stack == NULL) {
		sc_error_calc_alloc(stack, rpn);
	}
	while(!err_status && !rpn->is_empty(rpn)) {
		token = rpn->pop_front(rpn);
		if (token.type == NUMBER) {
			err_status = stack->push_front(stack, &token);
		} else if (token.type == VAR) {
			if (sc_get_variable(&var) == SC_VAR_SET) {
				token.type = NUMBER;
				token.value.num = var;
				err_status = stack->push_front(stack, &token);
			} else {
				err_status = SC_BAD_VAR;
			}
		} else if (token.type == UNARY_OP) {
			if (!stack->is_empty(stack)) {
				token_t operand = stack->pop_front(stack);
				unary_op[token.value.unary_op](&operand);
				err_status = stack->push_front(stack, &operand);
			} else {
				err_status = SC_BAD_TOKEN;
			}
		} else if (token.type == FUNCTION) {
			if (!stack->is_empty(stack)) {
				token_t operand = stack->pop_front(stack);
				function[token.value.func](&operand);
				err_status = stack->push_front(stack, &operand);
			} else {
				err_status = SC_BAD_TOKEN;
			}
		} else if (token.type == BINARY_OP) {
			if (!stack->is_empty(stack)) {
				token_t operand2 = stack->pop_front(stack);
				if (!stack->is_empty(stack)) {
					token_t operand1 = stack->pop_front(stack);
					binary_op[token.value.binary_op](&operand1, &operand2, &token);
					err_status = stack->push_front(stack, &token);
				} else {
					err_status = SC_BAD_TOKEN;
				}
			} else {
				err_status = SC_BAD_TOKEN;
			}
		} else {
			err_status = SC_BAD_TOKEN;
		}
	}
	if (err_status == SC_BAD_ALLOC) {
		sc_error_calc_alloc(stack, rpn);
	} else if (err_status == SC_BAD_VAR) {
		sc_error_calc_bad_var(stack, rpn);
	} else if (err_status == SC_BAD_TOKEN) {
		sc_error_calc_bad_token(stack, rpn);
	} else if (stack->is_empty(stack)) {
		sc_error_calc_bad_token(stack, rpn);
	} else if (stack->peek_front(stack)->type != NUMBER) {
		sc_error_calc_bad_token(stack, rpn);
	} else {
		*result = stack->pop_front(stack).value.num;
		if (!stack->is_empty(stack)) {
			sc_print_error(SC_CALC_ERROR_MESSAGE);
			err_status = SC_BAD_TOKEN;
		}
		stack->clear(stack);
		rpn->clear(rpn);
	}

	return (err_status);
}

/*
int expression_simplification(deque_t *rpn) {
	deque_t *stack
	token_t token;

	stack = create_deque();
	if (stack == NULL) {
		sc_error_calc_alloc(stack, rpn);
	}
	while (!rpn->is_empty(rpn)) {
		token = rpn->pop_front(rpn);
		if (token.type == NUMBER || token.type == VAR) {
			stack->push_front(stack, &token);
		} else if (token.type == UNARY_OP) {
			if (stack->is_empty(stack)) {
				// error
				(void) stack;
			} else if (stack->peek_front(stack)->type == NUMBER) {
				token_t operand = stack->pop_front(stack);
				unary_op[token.value.unary_op](&operand);
				err_status = stack->push_front(stack, &operand);
			} else {
				err_status = stack->push_front(stack, &token);
			}
		} else if (token.type == FUNCTION) {
			///
		} else if (token.type == BINARY_OP) {

		}
	}
}
*/

static void sc_minus(token_t *operand) {
	operand->value.num = -operand->value.num;
}

static void sc_plus(token_t *operand) {
	(void) operand;
}

static void sc_cos(token_t *operand) {
	operand->value.num = cos(operand->value.num);
}

static void sc_sin(token_t *operand) {
	operand->value.num = sin(operand->value.num);
}

static void sc_tan(token_t *operand) {
	operand->value.num = tan(operand->value.num);
}

static void sc_acos(token_t *operand) {
	operand->value.num = acos(operand->value.num);
}

static void sc_asin(token_t *operand) {
	operand->value.num = asin(operand->value.num);
}

static void sc_atan(token_t *operand) {
	operand->value.num = atan(operand->value.num);
}

static void sc_sqrt(token_t *operand) {
	operand->value.num = sqrt(operand->value.num);
}

static void sc_ln(token_t *operand) {
	operand->value.num = log(operand->value.num);
}

static void sc_log(token_t *operand) {
	operand->value.num = log(operand->value.num) / log(10.0);
}

static void sc_f(token_t *operand) {
	(void) operand;
}

static void sc_add(token_t *num1, token_t *num2, token_t *result) {
	result->type = NUMBER;
	result->value.num = num1->value.num + num2->value.num;
}

static void sc_sub(token_t *num1, token_t *num2, token_t *result) {
	result->type = NUMBER;
	result->value.num = num1->value.num - num2->value.num;
}

static void sc_mul(token_t *num1, token_t *num2, token_t *result) {
	result->type = NUMBER;
	result->value.num = num1->value.num * num2->value.num;
}

static void sc_div(token_t *num1, token_t *num2, token_t *result) {
	if (fabs(num2->value.num) - 1.0e-6 > 0) {
		result->type = NUMBER;
		result->value.num = num1->value.num / num2->value.num;
	} else {
		result->type = BAD_TOKEN;
	}
}

static void sc_mod(token_t *num1, token_t *num2, token_t *result) {
	result->type = NUMBER;
	result->value.num = fmod(num1->value.num, num2->value.num);
}

static void sc_pow(token_t *num1, token_t *num2, token_t *result) {
	result->type = NUMBER;
	result->value.num = pow(num1->value.num, num2->value.num);
}
