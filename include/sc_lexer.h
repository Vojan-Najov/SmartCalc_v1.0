#ifndef __SMARTCALC_LEXER_H__
#define __SMARTCALC_LEXER_H__

#include "sc_deque.h"

/*
  The lexer splits the input string into tokens and fills the queue with them.
  Support tokens:
    numbers, var (x, X), unary operators (-, +)
    binary operators (-, +, *, /, mod, ^),
    functions(sin(), cos(), tan(), asin(), acos(), atan(), log(), ln(), f(), F()).

  Lexer returns an integer. If there are no errors in the input string,
  0 is returned and lexems's deque created and this deque contains a list of tokens,
  otherwise the error code is returned and lexems contains NULL.

  Errors: SC_BAD_ALLOC, SC_BAD_TOKEN, SC_BAD_EXPR
*/

int sc_lexer(const char *str, sc_deque_t *lexems);

#endif
