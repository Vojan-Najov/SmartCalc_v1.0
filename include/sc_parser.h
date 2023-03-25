#ifndef _SMARTCALC_PARCER_H
#define _SMARTCALC_PARCER_H

#include "sc_deque.h"

/*
  The parser translates the input queue of tokens written in infix notation into 
  the output queue of tokens written in postfix notation - reverse Polish notation,
  using the "shunting yard" algorithm.
*/

sc_deque_t *parser(sc_deque_t *lexems);

#endif
