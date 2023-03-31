#ifndef _SMARTCALC_CALCULATOR_H
#define _SMARTCALC_CALCULATOR_H

#include "sc_deque.h"

#define SC_IS_EXPRESSION 1
#define SC_IS_DEFINITION 0

/*

  Errors: SC_BAD_ALLOC, SC_BAD_EXPR, SC_BAD_VAR, SC_BAD_FUNC,
          SC_RECURSIVE_FUNC, SC_DEVIDE_BY_ZERO.
*/

int sc_assignment(sc_deque_t *rpn);

int sc_definition(sc_deque_t *rpn);

int sc_calculation(sc_deque_t *rpn, double *result);

#endif
