#include "sc_parser.h"

#include <stddef.h>

#include "sc_error.h"

static int _check_priority(sc_token_t *token1, sc_token_t *token2);

static int _shunting_yard_algorithm(sc_deque_t *lexems, sc_deque_t *rpn,
                                    sc_deque_t *stack);

int sc_parser(sc_deque_t *lexems, sc_deque_t *rpn) {
  sc_deque_t stack;
  sc_token_t token;
  int err_status = 0;

  sc_init_deque(&stack);
  err_status = _shunting_yard_algorithm(lexems, rpn, &stack);
  while (!err_status && !stack.is_empty(&stack)) {
    token = stack.pop_front(&stack);
    if (token.type == SC_LBRACKET) {
      err_status = SC_BAD_BRACKET;
    } else {
      err_status = rpn->push_back(rpn, &token);
    }
  }
  stack.clear(&stack);

  return (err_status);
}

static int _shunting_yard_algorithm(sc_deque_t *lexems, sc_deque_t *rpn,
                                    sc_deque_t *stack) {
  int err_status = 0;
  sc_token_t token, tmp;

  while (!err_status && !lexems->is_empty(lexems)) {
    token = lexems->pop_front(lexems);
    if (token.type == SC_NUMBER || token.type == SC_VAR) {
      err_status = rpn->push_back(rpn, &token);
    } else if (token.type == SC_FUNCTION) {
      if (!lexems->is_empty(lexems) &&
          lexems->peek_front(lexems)->type == SC_LBRACKET) {
        err_status = stack->push_front(stack, &token);
      } else {
        err_status = SC_BAD_FUNCDEF;
      }
    } else if (token.type == SC_UNARY_OP || token.type == SC_LBRACKET) {
      if (!lexems->is_empty(lexems)) {
        err_status = stack->push_front(stack, &token);
      } else {
        err_status = SC_BAD_EXPR;
      }
    } else if (token.type == SC_BINARY_OP) {
      while (!err_status && !stack->is_empty(stack) &&
             _check_priority(&token, stack->peek_front(stack))) {
        tmp = stack->pop_front(stack);
        err_status = rpn->push_back(rpn, &tmp);
      }
      if (!lexems->is_empty(lexems)) {
        err_status = stack->push_front(stack, &token);
      } else {
        err_status = SC_BAD_EXPR;
      }
    } else if (token.type == SC_RBRACKET) {
      while (!err_status && !stack->is_empty(stack) &&
             stack->peek_front(stack)->type != SC_LBRACKET) {
        token = stack->pop_front(stack);
        err_status = rpn->push_back(rpn, &token);
      }
      if (!err_status && stack->is_empty(stack)) {
        err_status = SC_BAD_BRACKET;
      } else if (!err_status) {
        /* There is LBRACKET in the top of the stack */
        stack->pop_front(stack);
        if (!stack->is_empty(stack) &&
            (stack->peek_front(stack)->type == SC_UNARY_OP ||
             stack->peek_front(stack)->type == SC_FUNCTION)) {
          token = stack->pop_front(stack);
          err_status = rpn->push_back(rpn, &token);
        }
      }
    } else {
      err_status = SC_BAD_EXPR;
    }
  }

  return (err_status);
}

static int _check_priority(sc_token_t *token1, sc_token_t *token2) {
  int o1, o2;
  enum sc_associativity { SC_LEFT, SC_RIGHT };
  static int priority[] = {
      2,  // ADD
      2,  // SUB
      3,  // MUL
      3,  // DIV
      3,  // MOD
      4,  // POW
  };
  static int associativity[] = {
      SC_LEFT,   // ADD
      SC_LEFT,   // SUB
      SC_LEFT,   // MUL
      SC_LEFT,   // DIV
      SC_LEFT,   // MOD
      SC_RIGHT,  // POW
  };
  int ret = 0;

  if (token2->type == SC_BINARY_OP) {
    o1 = token1->value.binary_op;
    o2 = token2->value.binary_op;
    ret = priority[o2] > priority[o1] ||
          (priority[o2] == priority[o1] && associativity[o1] == SC_LEFT);
  } else if (token2->type == SC_FUNCTION) {
    ret = 1;
  } else if (token2->type == SC_UNARY_OP) {
    if (token1->value.binary_op == SC_POW) {
      ret = 0;
    } else {
      ret = 1;
    }
  }

  return (ret);
}
