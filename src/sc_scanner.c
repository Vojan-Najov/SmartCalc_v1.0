#include "sc_scanner.h"

#include "sc_error.h"

int sc_scanner(sc_deque_t *lexems) {
  int err_status = 0;
  sc_token_t token;
  int status = SC_EXPRESSION;

  token = lexems->pop_front(lexems);
  if (token.type == SC_VAR) {
    if (!lexems->is_empty(lexems) &&
        lexems->peek_front(lexems)->type == SC_ASSIGN) {
      lexems->pop_front(lexems);
      status = lexems->is_empty(lexems) ? SC_SCANNER_ERROR : SC_ASSIGNMENT;
    }
  } else if (token.type == SC_FUNCTION) {
    if (!lexems->is_empty(lexems) &&
        lexems->peek_front(lexems)->type == SC_ASSIGN) {
      if (token.value.func == SC_F) {
        lexems->pop_front(lexems);
        status = lexems->is_empty(lexems) ? SC_SCANNER_ERROR : SC_DEFINITION;
      } else {
        status = SC_SCANNER_ERROR;
      }
    }
  }
  if (status == SC_EXPRESSION) {
    err_status = lexems->push_front(lexems, &token);
  }
  if (err_status == SC_BAD_ALLOC) {
    sc_error_scanner(lexems);
  }

  return (status);
}
