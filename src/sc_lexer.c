#include "sc_lexer.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "sc_error.h"

static void get_token(const char *str, sc_token_t *token,
                      int prev_sc_token_type, const char **endptr);

static void get_symbolic_operator(char c, sc_token_t *token, int prev_token);

static void get_functional_operator(const char *str, sc_token_t *token,
                                    const char **endptr);

static int check_last_tokens(int cur_token, int prev_token);

int sc_lexer(const char *str, sc_deque_t *lexems) {
  sc_token_t token;
  int prev_token = SC_EMPTY_TOKEN;
  int err_status = 0;

  while (!err_status && *str) {
    get_token(str, &token, prev_token, &str);
    if (token.type == SC_WRONG_TOKEN) {
      err_status = SC_BAD_TOKEN;
    } else if (token.type != SC_EMPTY_TOKEN) {
      err_status = check_last_tokens(token.type, prev_token);
      if (!err_status) {
        err_status = lexems->push_back(lexems, &token);
      }
    }
    prev_token = token.type;
  }
  if (!err_status && lexems->is_empty(lexems)) {
    err_status = SC_NO_TOKENS;
  }

  return (err_status);
}

static int check_last_tokens(int cur_token, int prev_token) {
  int err_status = 0;

  if (cur_token == SC_NUMBER || cur_token == SC_VAR) {
    if (prev_token == SC_NUMBER || prev_token == SC_VAR ||
        prev_token == SC_RBRACKET) {
      err_status = SC_BAD_EXPR;
    }
    if (prev_token == SC_FUNCTION) {
      err_status = SC_BAD_FUNCDEF;
    }
  } else if (cur_token == SC_ASSIGN) {
    if (prev_token != SC_VAR && prev_token != SC_FUNCTION) {
      err_status = SC_BAD_EXPR;
    }
  } else if (cur_token == SC_UNARY_OP) {
    if (prev_token == SC_FUNCTION) {
      err_status = SC_BAD_EXPR;
    }
  } else if (cur_token == SC_BINARY_OP) {
    if (prev_token != SC_NUMBER && prev_token != SC_VAR &&
        prev_token != SC_RBRACKET) {
      err_status = SC_BAD_EXPR;
    }
  } else if (cur_token == SC_FUNCTION) {
    if (prev_token == SC_NUMBER || prev_token == SC_VAR ||
        prev_token == SC_FUNCTION || prev_token == SC_RBRACKET) {
      err_status = SC_BAD_EXPR;
    }
  } else if (cur_token == SC_LBRACKET) {
    if (prev_token == SC_NUMBER || prev_token == SC_VAR ||
        prev_token == SC_RBRACKET) {
      err_status = SC_BAD_EXPR;
    }
  } else if (cur_token == SC_RBRACKET) {
    if (prev_token != SC_NUMBER && prev_token != SC_VAR &&
        prev_token != SC_RBRACKET) {
      err_status = SC_BAD_EXPR;
    }
  }

  return (err_status);
}

static void get_token(const char *str, sc_token_t *token, int prev_token,
                      const char **endptr) {
  char *endtmp;

  while (isspace(*str)) {
    ++str;
  }
  if (*str == '\0') {
    token->type = SC_EMPTY_TOKEN;
  } else if (*str == '(') {
    token->type = SC_LBRACKET;
    ++str;
  } else if (*str == ')') {
    token->type = SC_RBRACKET;
    ++str;
  } else if (tolower(*str) == 'x') {
    token->type = SC_VAR;
    ++str;
  } else if (isdigit(*str) || (*str == '.' && isdigit(*(str + 1)))) {
    token->type = SC_NUMBER;
    token->value.num = strtod(str, &endtmp);
    str = endtmp;
  } else if (strchr("-+*/:%^=", *str) != NULL) {
    get_symbolic_operator(*str, token, prev_token);
    ++str;
  } else {
    get_functional_operator(str, token, &str);
  }

  *endptr = str;
}

static void get_symbolic_operator(char c, sc_token_t *token, int prev_token) {
  token->type = SC_BINARY_OP;
  if (c == '-') {
    if (prev_token == SC_NUMBER || prev_token == SC_VAR ||
        prev_token == SC_RBRACKET) {
      token->value.binary_op = SC_SUB;
    } else {
      token->type = SC_UNARY_OP;
      token->value.unary_op = SC_MINUS;
    }
  } else if (c == '+') {
    if (prev_token == SC_NUMBER || prev_token == SC_VAR ||
        prev_token == SC_RBRACKET) {
      token->value.binary_op = SC_ADD;
    } else {
      token->type = SC_UNARY_OP;
      token->value.unary_op = SC_PLUS;
    }
  } else if (c == '*') {
    token->value.binary_op = SC_MUL;
  } else if (c == '/' || c == ':') {
    token->value.binary_op = SC_DIV;
  } else if (c == '%') {
    token->value.binary_op = SC_MOD;
  } else if (c == '^') {
    token->value.binary_op = SC_POW;
  } else if (c == '=') {
    token->type = SC_ASSIGN;
  } else {
    token->type = SC_WRONG_TOKEN;
  }
}

static void get_functional_operator(const char *str, sc_token_t *token,
                                    const char **endptr) {
  token->type = SC_FUNCTION;
  if (strncmp(str, "sin", 3) == 0) {
    token->value.func = SC_SIN;
    str += 3;
  } else if (strncmp(str, "cos", 3) == 0) {
    token->value.func = SC_COS;
    str += 3;
  } else if (strncmp(str, "tan", 3) == 0) {
    token->value.func = SC_TAN;
    str += 3;
  } else if (strncmp(str, "asin", 4) == 0) {
    token->value.func = SC_ASIN;
    str += 4;
  } else if (strncmp(str, "acos", 4) == 0) {
    token->value.func = SC_ACOS;
    str += 4;
  } else if (strncmp(str, "atan", 4) == 0) {
    token->value.func = SC_ATAN;
    str += 4;
  } else if (strncmp(str, "sqrt", 4) == 0) {
    token->value.func = SC_SQRT;
    str += 4;
  } else if (strncmp(str, "ln", 2) == 0) {
    token->value.func = SC_LN;
    str += 2;
  } else if (strncmp(str, "log", 3) == 0) {
    token->value.func = SC_LOG;
    str += 3;
  } else if (strncmp(str, "mod", 3) == 0) {
    token->type = SC_BINARY_OP;
    token->value.binary_op = SC_MOD;
    str += 3;
  } else if (tolower(*str) == 'f') {
    token->value.func = SC_F;
    str += 1;
  } else {
    token->type = SC_WRONG_TOKEN;
  }

  *endptr = str;
}
