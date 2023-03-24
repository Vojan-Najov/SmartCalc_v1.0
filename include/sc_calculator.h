#ifndef _SMARTCALC_CALCULATOR_H
#define _SMARTCALC_CALCULATOR_H

#include "deque.h"

#define SC_IS_EXPRESSION 1
#define SC_IS_DEFINITION 0

int sc_calculate(deque_t *rpn, double *result);

int sc_definition(deque_t *rpn);

#endif