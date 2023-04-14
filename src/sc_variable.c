#include "sc_variable.h"

static int *_get_variable_status(void) {
	static int var_status = SC_VAR_UNSET;

	return (&var_status);
}

static double *_get_variable_address(void) {
	static double var = 0.0;

	return (&var);
}

static int *_get_saved_variable_status(void) {
	static int var_status = SC_VAR_UNSET;

	return (&var_status);
}

static double *_get_saved_variable_address(void) {
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

void sc_unset_variable(void) {
	int *var_status_ptr;

	var_status_ptr = _get_variable_status();
	*var_status_ptr = SC_VAR_UNSET;
}

int sc_get_variable(double *x) {
	int *var_status_ptr;
	double *var_ptr;

	var_ptr = _get_variable_address();
	var_status_ptr = _get_variable_status();
	
	*x = *var_ptr;

	return (*var_status_ptr);
}

void sc_save_variable(void) {
	int *var_status_ptr;
	double *var_ptr;
	int *saved_var_status_ptr;
	double *saved_var_ptr;

	var_ptr = _get_variable_address();
	var_status_ptr = _get_variable_status();
	saved_var_ptr = _get_saved_variable_address();
	saved_var_status_ptr = _get_saved_variable_status();
	
	if (*var_status_ptr == SC_VAR_SET) {
		*saved_var_status_ptr = SC_VAR_SET;
		*saved_var_ptr = *var_ptr;
	}
}

void sc_restore_variable(void) {
	int *var_status_ptr;
	double *var_ptr;
	int *saved_var_status_ptr;
	double *saved_var_ptr;

	var_ptr = _get_variable_address();
	var_status_ptr = _get_variable_status();
	saved_var_ptr = _get_saved_variable_address();
	saved_var_status_ptr = _get_saved_variable_status();

	if (*saved_var_status_ptr == SC_VAR_SET) {
		*var_status_ptr = SC_VAR_SET;
		*var_ptr = *saved_var_ptr;
	} else {
		*var_status_ptr = SC_VAR_UNSET;
	}
}

