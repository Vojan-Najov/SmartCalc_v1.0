#ifndef _SMARTCALC_VARIABLE_H
#define _SMARTCALC_VARIABLE_H

#define SC_VAR_SET 1
#define SC_VAR_UNSET 0

void sc_set_variable(double x);

void sc_unset_variable(void);

int sc_get_variable(double *x);

#endif
