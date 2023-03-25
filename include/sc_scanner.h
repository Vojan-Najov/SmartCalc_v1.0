#ifndef _SMARTCALC_SCANNER_H
#define _SMARTCALC_SCANNER_H

#include "sc_deque.h"

#define SC_ASSIGNMENT 1
#define SC_EXPRESSION 2
#define SC_DEFINITION 3
#define SC_BAD_EXPR -1

/*
  The scanner determines by the first two tokens what type the token input queue has.
  It can be 
    an expression, or
    an assignment to a variable (if the first tokens are 'x' and '='), or
    a function definition (if the first tokens are 'f' and '='), or
    an error.
*/

int sc_scanner(sc_deque_t *lexems);

#endif
