#include "sc_variable.h"

static int *_get_variable_status(void) {
	static int var_status = SC_VAR_UNSET;

	return (&var_status);
}

static double *_get_variable_address(void) {
	static double var = 0.0;

	return (&var);
}

void sc_set_variable(double x) {
	int *var_status_ptr;
	double *var_ptr;

	var_ptr = _get_variable_address();
	var_status_ptr = _get_variable_status();

	*var_ptr = x;
	*var_status_ptr = SC_VAR_SET;
}

int sc_get_variable(double *x) {
	int *var_status_ptr;
	double *var_ptr;

	var_ptr = _get_variable_address();
	var_status_ptr = _get_variable_status();
	
	*x = *var_ptr;

	return (*var_status_ptr);
}
