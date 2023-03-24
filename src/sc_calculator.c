#include "sc_calculator.h"
#include "sc_error.h"
#include "sc_variable.h"
#include "sc_function.h"
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

static int sc_calculate_aux(deque_t *rpn, deque_t *stack, int is_expr);
static int sc_expr_handle_var(deque_t *stack);
static int sc_expr_handle_unary_op(deque_t *stack, int unary_op);
static int sc_def_handle_unary_op(deque_t *stack, token_t *token);
static int sc_expr_handle_function(deque_t *stack, int function);
static int sc_def_handle_function(deque_t *stack, token_t *token);
static int sc_expr_handle_binary_op(deque_t *stack, int binary_op);
static int sc_def_handle_binary_op(deque_t *stack, token_t *token);

static void (*unary_op_array[2])(token_t *) = {
	&sc_minus,
	&sc_plus,
};

static void (*function_array[])(token_t *) = {
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

static void (*binary_op_array[])(token_t *, token_t *, token_t *) = {
	&sc_add,
	&sc_sub,
	&sc_mul,
	&sc_div,
	&sc_mod,
	&sc_pow,
};

int sc_calculate(deque_t *rpn, double *result) {
	deque_t *stack;
	int err_status = 0;

	stack = create_deque();
	if (stack == NULL) {
		sc_error_calc_alloc(stack, rpn);
	}
	err_status = sc_calculate_aux(rpn, stack, SC_IS_EXPRESSION);
	if (!err_status) {
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

int sc_definition(deque_t *rpn) {
	deque_t *stack;
	int err_status = 0;

	stack = create_deque();
	if (stack == NULL) {
		sc_error_calc_alloc(stack, rpn);
	}
	err_status = sc_calculate_aux(rpn, stack, SC_IS_DEFINITION);
	if (!err_status) {
		stack->reverse(stack);
		err_status = sc_set_function(stack);
		stack->clear(stack);
		rpn->clear(rpn);
	}

	return (err_status);
}

static int sc_calculate_aux(deque_t *rpn, deque_t *stack, int is_expr) {
	token_t token;
	int err_status = 0;

	while(!err_status && !rpn->is_empty(rpn)) {
		token = rpn->pop_front(rpn);
		if (token.type == NUMBER) {
			err_status = stack->push_front(stack, &token);
		} else if (token.type == VAR) {
			if (is_expr) {
				err_status = sc_expr_handle_var(stack);
			} else {
				err_status = stack->push_front(stack, &token);
			}
		} else if (token.type == UNARY_OP) {
			if (is_expr) {
				err_status = sc_expr_handle_unary_op(stack, token.value.unary_op);
			} else {
				err_status = sc_def_handle_unary_op(stack, &token);
			}
		} else if (token.type == FUNCTION) {
			if (is_expr) {
				err_status = sc_expr_handle_function(stack, token.value.func);
			} else {
				err_status = sc_def_handle_function(stack, &token);
			}
		} else if (token.type == BINARY_OP) {
			if (is_expr) {
				err_status = sc_expr_handle_binary_op(stack, token.value.binary_op);
			} else {
				err_status = sc_def_handle_binary_op(stack, &token);
			}
		} else {
			err_status = SC_BAD_TOKEN;
		}
	}
	if (err_status == SC_BAD_ALLOC) {
		sc_error_calc_alloc(stack, rpn);
	} else if (err_status == SC_DEVIDE_BY_ZERO) {
		sc_error_calc_devide_by_zero(stack, rpn);
	} else if (err_status == SC_BAD_VAR) {
		sc_error_calc_bad_var(stack, rpn);
	} else if (err_status == SC_BAD_FUNC) {
		sc_error_calc_bad_func(stack, rpn);
	} else if (err_status == SC_BAD_TOKEN) {
		sc_error_calc_bad_token(stack, rpn);
	} else if (stack->is_empty(stack)) {
		sc_error_calc_bad_token(stack, rpn);
	} else if (is_expr && stack->peek_front(stack)->type != NUMBER) {
		sc_error_calc_bad_token(stack, rpn);
	}

	return (err_status);
}

static int sc_expr_handle_var(deque_t *stack) {
	double var;
	int err_status;
	token_t token;

	if (sc_get_variable(&var) == SC_VAR_SET) {
		token.type = NUMBER;
		token.value.num = var;
		err_status = stack->push_front(stack, &token);
	} else {
		err_status = SC_BAD_VAR;
	}

	return (err_status);
}

static int sc_expr_handle_unary_op(deque_t *stack, int unary_op) {
	token_t operand;
	int err_status;

	if (!stack->is_empty(stack)) {
		operand = stack->pop_front(stack);
		if (operand.type == NUMBER) {
			unary_op_array[unary_op](&operand);
			err_status = stack->push_front(stack, &operand);
		} else {
			err_status = SC_BAD_TOKEN;
		}
	} else {
		err_status = SC_BAD_TOKEN;
	}

	return (err_status);
}

static int sc_def_handle_unary_op(deque_t *stack, token_t *token) {
	token_t operand;
	int err_status;

	if (!stack->is_empty(stack)) {
		if (stack->peek_front(stack)->type == NUMBER) {
			operand = stack->pop_front(stack);
			unary_op_array[token->value.unary_op](&operand);
			err_status = stack->push_front(stack, &operand);
		} else {
			err_status = stack->push_front(stack, token);
		}
	} else {
		err_status = SC_BAD_TOKEN;
	}

	return (err_status);
}

static int sc_expr_handle_function(deque_t *stack, int function) {
	token_t operand;
	int err_status;

	if (!stack->is_empty(stack)) {
		operand = stack->pop_front(stack);
		if (operand.type == NUMBER) {
			function_array[function](&operand);
			err_status = stack->push_front(stack, &operand);
		} else {
			err_status = SC_BAD_TOKEN;
		}
	} else {
		err_status = SC_BAD_TOKEN;
	}

	return (err_status);
}

static int sc_def_handle_function(deque_t *stack, token_t *token) {
	deque_t *func_def;
	token_t operand;
	int err_status;

	if (!stack->is_empty(stack)) {
		if (stack->peek_front(stack)->type == NUMBER) {
			operand = stack->pop_front(stack);
			function_array[token->value.func](&operand);
			err_status = stack->push_front(stack, &operand);
		} else {
			if (token->value.func == F) {
				if (sc_get_function(&func_def) == SC_FUNC_SET) {
					while (!err_status && !func_def->is_empty(func_def)) {
						token_t tmp = func_def->pop_front(func_def);
						err_status = stack->push_front(stack, &tmp);
					}
					func_def->clear(func_def);
				}
			} else {
				err_status = stack->push_front(stack, token);
			}
		}
	} else {
		err_status = SC_BAD_TOKEN;
	}

	return (err_status);
}

static int sc_expr_handle_binary_op(deque_t *stack, int binary_op) {
	int err_status;
	token_t token;
	token_t operand1, operand2;

	if (!stack->is_empty(stack)) {
		operand2 = stack->pop_front(stack);
		if (!stack->is_empty(stack)) {
			operand1 = stack->pop_front(stack);
			if (operand1.type == NUMBER && operand2.type == NUMBER) {
				binary_op_array[binary_op](&operand1, &operand2, &token);
				if (token.type == NUMBER) {
					err_status = stack->push_front(stack, &token);
				} else {
					err_status = SC_DEVIDE_BY_ZERO;
				}
			} else {
				err_status = SC_BAD_TOKEN;
			}
		} else {
			err_status = SC_BAD_TOKEN;
		}
	} else {
		err_status = SC_BAD_TOKEN;
	}

	return (err_status);
}

static int sc_def_handle_binary_op(deque_t *stack, token_t *token) {
	int err_status;
	token_t operand1, operand2;

	if (!stack->is_empty(stack)) {
		operand2 = stack->pop_front(stack);
		if (!stack->is_empty(stack)) {
			operand1 = stack->pop_front(stack);
			if (operand1.type == NUMBER && operand2.type == NUMBER) {
				binary_op_array[token->value.binary_op](&operand1, &operand2, token);
				if (token->type == NUMBER) {
					err_status = stack->push_front(stack, token);
				} else {
					err_status = SC_DEVIDE_BY_ZERO;
				}
			} else {
				err_status = stack->push_front(stack, &operand1);
				err_status = stack->push_front(stack, &operand2);
				err_status = stack->push_front(stack, token);
			}
		} else {
			err_status = SC_BAD_TOKEN;
		}
	} else {
		err_status = SC_BAD_TOKEN;
	}

	return (err_status);
}

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
	deque_t *func_def;
	int err_status;
	double result;
	double var;
	int var_status = SC_VAR_UNSET;

	err_status = sc_get_function(&func_def);
	if (func_def != NULL) {
		if (sc_get_variable(&var) == SC_VAR_SET) {
			var_status = SC_VAR_SET;
		}
		sc_set_variable(operand->value.num);
		err_status = sc_calculate(func_def, &result);
		operand->value.num = result;
		if (var_status == SC_VAR_UNSET) {
			sc_unset_variable();
		} else {
			sc_set_variable(var);
		}
	} else if (!err_status){
		err_status = SC_BAD_FUNC;
	}
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
	if (fabs(num2->value.num) - 1.0e-6 > 0) {
		result->type = NUMBER;
		result->value.num = fmod(num1->value.num, num2->value.num);
	} else {
		result->type = BAD_TOKEN;
	}
}

static void sc_pow(token_t *num1, token_t *num2, token_t *result) {
	result->type = NUMBER;
	result->value.num = pow(num1->value.num, num2->value.num);
}
