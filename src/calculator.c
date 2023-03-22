#include "error.h"
#include <math.h>
#include <stdlib.h>

static void smartcalc_minus(token_t *num);
static void smartcalc_plus(token_t *num);

static void smartcalc_cos(token_t *num);
static void smartcalc_sin(token_t *num);
static void smartcalc_tan(token_t *num);
static void smartcalc_acos(token_t *num);
static void smartcalc_asin(token_t *num);
static void smartcalc_atan(token_t *num);
static void smartcalc_sqrt(token_t *num);
static void smartcalc_ln(token_t *num);
static void smartcalc_log(token_t *num);

static void smartcalc_add(token_t *num1, token_t *num2, token_t *result);
static void smartcalc_sub(token_t *num1, token_t *num2, token_t *result);
static void smartcalc_mul(token_t *num1, token_t *num2, token_t *result);
static void smartcalc_div(token_t *num1, token_t *num2, token_t *result);
static void smartcalc_mod(token_t *num1, token_t *num2, token_t *result);
static void smartcalc_pow(token_t *num1, token_t *num2, token_t *result);

static void (*unary_op[2])(token_t *) = {
	&smartcalc_minus,
	&smartcalc_plus,
};

static void (*function[])(token_t *) = {
	&smartcalc_cos,
	&smartcalc_sin,
	&smartcalc_tan,
	&smartcalc_acos,
	&smartcalc_asin,
	&smartcalc_atan,
	&smartcalc_sqrt,
	&smartcalc_ln,
	&smartcalc_log,
};

static void (*binary_op[])(token_t *, token_t *, token_t *) = {
	&smartcalc_add,
	&smartcalc_sub,
	&smartcalc_mul,
	&smartcalc_div,
	&smartcalc_mod,
	&smartcalc_pow,
};

int calculate(deque_t *rpn, double *result) {
	deque_t *stack;
	token_t token;
	int err_status = 0;

	stack = create_deque();
	if (stack == NULL) {
		error_calculate_alloc(stack, rpn);
	} else {
		while(!err_status && !rpn->is_empty(rpn)) {
			token = rpn->pop_front(rpn);
			if (token.type == NUMBER) {
				err_status = stack->push_front(stack, &token);
			} else if (token.type == UNARY_OP) {
				token_t operand = stack->pop_front(stack);
				unary_op[token.value.unary_op](&operand);
				err_status = stack->push_front(stack, &operand);
			} else if (token.type == FUNCTION) {
				token_t operand = stack->pop_front(stack);
				function[token.value.func](&operand);
				err_status = stack->push_front(stack, &operand);
			} else if (token.type == BINARY_OP) {
				token_t operand2 = stack->pop_front(stack);
				token_t operand1 = stack->pop_front(stack);
				binary_op[token.value.binary_op](&operand1, &operand2, &token);
				err_status = stack->push_front(stack, &token);
			} else {
				err_status = error_calculate_bad_token(stack, rpn);
			}
		}
		if (!err_status) {
			*result = stack->pop_front(stack).value.num;
			free(stack);
			free(rpn);
		} else if (err_status == SMARTCALC_BAD_ALLOC) {
			error_calculate_alloc(stack, rpn);
			*result = nan("");
		}
	}

	return (err_status);
}

static void smartcalc_minus(token_t *operand) {
	operand->value.num = -operand->value.num;
}

static void smartcalc_plus(token_t *operand) {
	(void) operand;
}

static void smartcalc_cos(token_t *operand) {
	operand->value.num = cos(operand->value.num);
}

static void smartcalc_sin(token_t *operand) {
	operand->value.num = sin(operand->value.num);
}

static void smartcalc_tan(token_t *operand) {
	operand->value.num = tan(operand->value.num);
}

static void smartcalc_acos(token_t *operand) {
	operand->value.num = acos(operand->value.num);
}

static void smartcalc_asin(token_t *operand) {
	operand->value.num = asin(operand->value.num);
}

static void smartcalc_atan(token_t *operand) {
	operand->value.num = atan(operand->value.num);
}

static void smartcalc_sqrt(token_t *operand) {
	operand->value.num = sqrt(operand->value.num);
}

static void smartcalc_ln(token_t *operand) {
	operand->value.num = log(operand->value.num);
}

static void smartcalc_log(token_t *operand) {
	operand->value.num = log(operand->value.num) / log(10.0);
}

static void smartcalc_add(token_t *num1, token_t *num2, token_t *result) {
	result->type = NUMBER;
	result->value.num = num1->value.num + num2->value.num;
}

static void smartcalc_sub(token_t *num1, token_t *num2, token_t *result) {
	result->type = NUMBER;
	result->value.num = num1->value.num - num2->value.num;
}

static void smartcalc_mul(token_t *num1, token_t *num2, token_t *result) {
	result->type = NUMBER;
	result->value.num = num1->value.num * num2->value.num;
}

static void smartcalc_div(token_t *num1, token_t *num2, token_t *result) {
	if (fabs(num2->value.num) - 1.0e-6 > 0) {
		result->type = NUMBER;
		result->value.num = num1->value.num / num2->value.num;
	} else {
		result->type = BAD_TOKEN;
	}
}

static void smartcalc_mod(token_t *num1, token_t *num2, token_t *result) {
	result->type = NUMBER;
	result->value.num = fmod(num1->value.num, num2->value.num);
}

static void smartcalc_pow(token_t *num1, token_t *num2, token_t *result) {
	result->type = NUMBER;
	result->value.num = pow(num1->value.num, num2->value.num);
}
