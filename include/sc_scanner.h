#ifndef _SMARTCALC_SCANNER_H
#define _SMARTCALC_SCANNER_H

#include "sc_deque.h"

#define SC_ASSIGNMENT 1
#define SC_EXPRESSION 2
#define SC_DEFINITION 3
#define SC_BAD_EXPR -1

int sc_scanner(sc_deque_t *lexems);

#endif
