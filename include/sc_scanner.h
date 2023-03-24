#ifndef _SMARTCALC_SCANNER_H
#define _SMARTCALC_SCANNER_H

#include "deque.h"

#define SC_ASSIGNMENT 1
#define SC_EXPRESSION 2
#define SC_DEFINITION 3
#define SC_BAD_EXPR -1

int sc_scanner(deque_t *lexems);

#endif
