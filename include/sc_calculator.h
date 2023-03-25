#ifndef _SMARTCALC_CALCULATOR_H
#define _SMARTCALC_CALCULATOR_H

#include "sc_deque.h"

#define SC_IS_EXPRESSION 1
#define SC_IS_DEFINITION 0

int sc_calculate(sc_deque_t *rpn, double *result);

int sc_definition(sc_deque_t *rpn);

#endif
