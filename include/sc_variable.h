#ifndef _SMARTCALC_VARIABLE_H
#define _SMARTCALC_VARIABLE_H

#define SC_VAR_SET 1
#define SC_VAR_UNSET 0

/*
  The calculator supports only one variable 'x' ('X').
  Here is an interface to set the value of the variable,
                       to reset the value of the variable,
                       to get the value of the set variable.
*/

void sc_set_variable(double x);

void sc_unset_variable(void);

int sc_get_variable(double *x);

void sc_save_variable(void);

void sc_restore_variable(void);

#endif
