#ifndef _SMARTCALC_LEXER_H
#define _SMARTCALC_LEXER_H

#include "sc_deque.h"

/*
  The lexer splits the input string into tokens and fills the queue with them.
  Support tokens: numbers, var (x, X), unary operators (-, +)
  binary operators (-, +, *, /, mod, ^),
  functions(sin(), cos(), tan(), asin(), acos(), atan(), log(), ln(), f(), F()).
*/

sc_deque_t *sc_lexer(const char *str);

#endif
