#include "sc_calculator.h"

#include <math.h>
#include <stdlib.h>

#include "sc_error.h"
#include "sc_function.h"
#include "sc_variable.h"

static void sc_minus(sc_token_t *num);
static void sc_plus(sc_token_t *num);

static void sc_cos(sc_token_t *num);
static void sc_sin(sc_token_t *num);
static void sc_tan(sc_token_t *num);
static void sc_acos(sc_token_t *num);
static void sc_asin(sc_token_t *num);
static void sc_atan(sc_token_t *num);
static void sc_sqrt(sc_token_t *num);
static void sc_ln(sc_token_t *num);
static void sc_log(sc_token_t *num);
static void sc_f(sc_token_t *num);

static void sc_add(sc_token_t *num1, sc_token_t *num2, sc_token_t *result);
static void sc_sub(sc_token_t *num1, sc_token_t *num2, sc_token_t *result);
static void sc_mul(sc_token_t *num1, sc_token_t *num2, sc_token_t *result);
static void sc_div(sc_token_t *num1, sc_token_t *num2, sc_token_t *result);
static void sc_mod(sc_token_t *num1, sc_token_t *num2, sc_token_t *result);
static void sc_pow(sc_token_t *num1, sc_token_t *num2, sc_token_t *result);

static int sc_calculate_aux(sc_deque_t *rpn, sc_deque_t *stack, int is_expr);

static int sc_handle_var(sc_deque_t *stack, sc_token_t *token, int is_expr);

static int sc_handle_unary_op(sc_deque_t *stack, sc_token_t *token,
                              int is_expr);

static int sc_handle_function(sc_deque_t *stack, sc_token_t *token,
                              int is_expr);

static int sc_handle_binary_op(sc_deque_t *stack, sc_token_t *token,
                               int is_expr);

static void (*unary_op_array[2])(sc_token_t *) = {
    &sc_minus,
    &sc_plus,
};

static void (*function_array[])(sc_token_t *) = {
    &sc_cos,  &sc_sin,  &sc_tan, &sc_acos, &sc_asin,
    &sc_atan, &sc_sqrt, &sc_ln,  &sc_log,  &sc_f,
};

static void (*binary_op_array[])(sc_token_t *, sc_token_t *, sc_token_t *) = {
    &sc_add, &sc_sub, &sc_mul, &sc_div, &sc_mod, &sc_pow,
};

int sc_assignment(sc_deque_t *rpn) {
  sc_deque_t stack;
  sc_token_t token;
  int err_status = 0;

  sc_init_deque(&stack);
  err_status = sc_calculate_aux(rpn, &stack, SC_IS_EXPRESSION);
  if (!err_status) {
    token = stack.pop_front(&stack);
    if (!stack.is_empty(&stack)) {
      err_status = SC_BAD_EXPR;
    } else {
      sc_set_variable(token.value.num);
    }
  }
  stack.clear(&stack);

  return (err_status);
}

int sc_definition(sc_deque_t *rpn) {
  sc_deque_t stack;
  int err_status = 0;

  sc_init_deque(&stack);
  err_status = sc_calculate_aux(rpn, &stack, SC_IS_DEFINITION);
  if (!err_status) {
    stack.reverse(&stack);
    err_status = sc_set_function(&stack);
  }
  stack.clear(&stack);

  return (err_status);
}

int sc_calculation(sc_deque_t *rpn, double *result) {
  sc_deque_t stack;
  sc_token_t token;
  int err_status = 0;

  sc_init_deque(&stack);
  err_status = sc_calculate_aux(rpn, &stack, SC_IS_EXPRESSION);
  if (!err_status) {
    token = stack.pop_front(&stack);
    if (!stack.is_empty(&stack)) {
      err_status = SC_BAD_EXPR;
    } else {
      *result = token.value.num;
    }
  }
  stack.clear(&stack);

  return (err_status);
}

static int sc_calculate_aux(sc_deque_t *rpn, sc_deque_t *stack, int is_expr) {
  sc_token_t token;
  int err_status = 0;

  while (!err_status && !rpn->is_empty(rpn)) {
    token = rpn->pop_front(rpn);
    if (token.type == SC_NUMBER) {
      err_status = stack->push_front(stack, &token);
    } else if (token.type == SC_VAR) {
      err_status = sc_handle_var(stack, &token, is_expr);
    } else if (token.type == SC_UNARY_OP) {
      err_status = sc_handle_unary_op(stack, &token, is_expr);
    } else if (token.type == SC_FUNCTION) {
      err_status = sc_handle_function(stack, &token, is_expr);
    } else if (token.type == SC_BINARY_OP) {
      err_status = sc_handle_binary_op(stack, &token, is_expr);
    } else {
      err_status = SC_BAD_EXPR;
    }
  }
  if (!err_status) {
    if (stack->is_empty(stack)) {
      err_status = SC_BAD_EXPR;
    } else if (is_expr && stack->peek_front(stack)->type != SC_NUMBER) {
      err_status = SC_BAD_EXPR;
    }
  }

  return (err_status);
}

static int sc_handle_var(sc_deque_t *stack, sc_token_t *token, int is_expr) {
  double var;
  int err_status;

  if (is_expr) {
    if (sc_get_variable(&var) == SC_VAR_SET) {
      token->type = SC_NUMBER;
      token->value.num = var;
      err_status = stack->push_front(stack, token);
    } else {
      err_status = SC_BAD_VAR;
    }
  } else {
    err_status = stack->push_front(stack, token);
  }

  return (err_status);
}

static int sc_handle_unary_op(sc_deque_t *stack, sc_token_t *token,
                              int is_expr) {
  sc_token_t operand;
  int err_status;

  if (!stack->is_empty(stack)) {
    if (stack->peek_front(stack)->type == SC_NUMBER) {
      operand = stack->pop_front(stack);
      unary_op_array[token->value.unary_op](&operand);
      err_status = stack->push_front(stack, &operand);
    } else {
      if (is_expr) {
        err_status = SC_BAD_EXPR;
      } else {
        err_status = stack->push_front(stack, token);
      }
    }
  } else {
    err_status = SC_BAD_EXPR;
  }

  return (err_status);
}

static int sc_handle_function(sc_deque_t *stack, sc_token_t *token,
                              int is_expr) {
  sc_token_t operand;
  int err_status;

  if (!stack->is_empty(stack)) {
    if (stack->peek_front(stack)->type == SC_NUMBER) {
      operand = stack->pop_front(stack);
      function_array[token->value.func](&operand);
      if (operand.type == SC_WRONG_TOKEN) {
        err_status = operand.value.error;
      } else {
        err_status = stack->push_front(stack, &operand);
      }
    } else {
      if (is_expr) {
        err_status = SC_BAD_EXPR;
      } else {
        if (token->value.func != SC_F) {
          err_status = stack->push_front(stack, token);
        } else {
          err_status = SC_RECURSIVE_FUNC;
        }
      }
    }
  } else {
    err_status = SC_BAD_EXPR;
  }

  return (err_status);
}

static int sc_handle_binary_op(sc_deque_t *stack, sc_token_t *token,
                               int is_expr) {
  int err_status;
  sc_token_t operand1, operand2;

  if (!stack->is_empty(stack)) {
    operand2 = stack->pop_front(stack);
    if (!stack->is_empty(stack)) {
      operand1 = stack->pop_front(stack);
      if (operand1.type == SC_NUMBER && operand2.type == SC_NUMBER) {
        binary_op_array[token->value.binary_op](&operand1, &operand2, token);
        if (token->type == SC_WRONG_TOKEN) {
          err_status = token->value.error;
        } else {
          err_status = stack->push_front(stack, token);
        }
      } else {
        if (is_expr) {
          err_status = SC_BAD_TOKEN;
        } else {
          err_status = stack->push_front(stack, &operand1);
          err_status = stack->push_front(stack, &operand2);
          err_status = stack->push_front(stack, token);
        }
      }
    } else {
      err_status = SC_BAD_EXPR;
    }
  } else {
    err_status = SC_BAD_EXPR;
  }

  return (err_status);
}

static void sc_minus(sc_token_t *operand) {
  operand->value.num = -operand->value.num;
}

static void sc_plus(sc_token_t *operand) { (void)operand; }

static void sc_cos(sc_token_t *operand) {
  operand->value.num = cos(operand->value.num);
}

static void sc_sin(sc_token_t *operand) {
  operand->value.num = sin(operand->value.num);
}

static void sc_tan(sc_token_t *operand) {
  operand->value.num = tan(operand->value.num);
}

static void sc_acos(sc_token_t *operand) {
  operand->value.num = acos(operand->value.num);
}

static void sc_asin(sc_token_t *operand) {
  operand->value.num = asin(operand->value.num);
}

static void sc_atan(sc_token_t *operand) {
  operand->value.num = atan(operand->value.num);
}

static void sc_sqrt(sc_token_t *operand) {
  operand->value.num = sqrt(operand->value.num);
}

static void sc_ln(sc_token_t *operand) {
  operand->value.num = log(operand->value.num);
}

static void sc_log(sc_token_t *operand) {
  operand->value.num = log(operand->value.num) / log(10.0);
}

static void sc_f(sc_token_t *operand) {
  sc_deque_t *func_def;
  sc_deque_t stack;
  int err_status = 0;

  sc_init_deque(&stack);
  err_status = sc_get_function(&func_def);
  if (!err_status) {
    sc_save_variable();
    sc_set_variable(operand->value.num);
    err_status = sc_calculate_aux(func_def, &stack, SC_IS_EXPRESSION);
    if (!err_status) {
      *operand = stack.pop_front(&stack);
      if (!stack.is_empty(&stack)) {
        err_status = SC_BAD_EXPR;
      }
    }
    stack.clear(&stack);
    sc_destroy_deque(func_def);
    sc_restore_variable();
  }
  if (err_status) {
    operand->type = SC_WRONG_TOKEN;
    operand->value.error = err_status;
  }
}

static void sc_add(sc_token_t *num1, sc_token_t *num2, sc_token_t *result) {
  result->type = SC_NUMBER;
  result->value.num = num1->value.num + num2->value.num;
}

static void sc_sub(sc_token_t *num1, sc_token_t *num2, sc_token_t *result) {
  result->type = SC_NUMBER;
  result->value.num = num1->value.num - num2->value.num;
}

static void sc_mul(sc_token_t *num1, sc_token_t *num2, sc_token_t *result) {
  result->type = SC_NUMBER;
  result->value.num = num1->value.num * num2->value.num;
}

static void sc_div(sc_token_t *num1, sc_token_t *num2, sc_token_t *result) {
  if (fabs(num2->value.num) - 1.0e-6 > 0) {
    result->type = SC_NUMBER;
    result->value.num = num1->value.num / num2->value.num;
  } else {
    result->type = SC_WRONG_TOKEN;
    result->value.error = SC_DEVIDE_BY_ZERO;
  }
}

static void sc_mod(sc_token_t *num1, sc_token_t *num2, sc_token_t *result) {
  if (fabs(num2->value.num) - 1.0e-6 > 0) {
    result->type = SC_NUMBER;
    result->value.num = fmod(num1->value.num, num2->value.num);
  } else {
    result->type = SC_WRONG_TOKEN;
    result->value.error = SC_DEVIDE_BY_ZERO;
  }
}

static void sc_pow(sc_token_t *num1, sc_token_t *num2, sc_token_t *result) {
  result->type = SC_NUMBER;
  result->value.num = pow(num1->value.num, num2->value.num);
}
