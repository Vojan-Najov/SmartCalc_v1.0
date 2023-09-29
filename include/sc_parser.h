#ifndef _SMARTCALC_PARCER_H
#define _SMARTCALC_PARCER_H

#include "sc_deque.h"

/*
  The parser translates the input queue of tokens written in infix notation into
  the output queue of tokens written in postfix notation - reverse Polish
  notation, using the "shunting yard" algorithm.

  Errors: SC_BAD_ALLOC, SC_BAD_EXPR, SC_BAD_FUNCDEF, SC_BAD_BRACKET
*/

int sc_parser(sc_deque_t *lexems, sc_deque_t *rpn);

#endif
