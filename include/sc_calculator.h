#ifndef _SMARTCALC_CALCULATOR_H
#define _SMARTCALC_CALCULATOR_H

#include "sc_deque.h"

#define SC_IS_EXPRESSION 1
#define SC_IS_DEFINITION 0

void sc_assignment(sc_deque_t *rpn);

void sc_definition(sc_deque_t *rpn);

int sc_calculation(sc_deque_t *rpn, double *result);

#endif
