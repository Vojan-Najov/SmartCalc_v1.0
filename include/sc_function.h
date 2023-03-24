#ifndef _SMARTCALC_FUNCTION_H
#define _SMARTCALC_FUNCTION_H

#include "deque.h"

#define SC_FUNC_SET 1
#define SC_FUNC_UNSET 0

int sc_set_function(deque_t *func_def);

void sc_unset_function(void);

int sc_get_function(deque_t **func_def);

#endif