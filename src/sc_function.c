#include "sc_function.h"
#include "sc_error.h"
#include <stddef.h>

static int *_get_function_status(void) {
	static int func_status = SC_FUNC_UNSET;

	return (&func_status);
}

static sc_deque_t **_get_function_address(void) {
	static sc_deque_t *func_def = NULL;

	return (&func_def);
}

int sc_set_function(sc_deque_t *func_def) {
	int err_status = 0;
	int *func_status;
	sc_deque_t **func_ptr;

	func_status = _get_function_status();
	func_ptr = _get_function_address();
	sc_unset_function();
	(*func_ptr) = sc_copy_deque(func_def);
	if (*func_ptr == NULL) {
		err_status = SC_BAD_ALLOC;
	} else {
		*func_status = SC_FUNC_SET;
	}

	return (err_status);
}

void sc_unset_function(void) {
	int *func_status;
	sc_deque_t **func_ptr;
	
	func_status = _get_function_status();
	if (*func_status == SC_FUNC_SET) {
		func_ptr = _get_function_address();
		(*func_ptr)->clear(*func_ptr);
		*func_ptr = NULL;
		*func_status = SC_FUNC_UNSET;
	}
}

int sc_get_function(sc_deque_t **func_def) {
	int err_status = 0;
	int *func_status;
	sc_deque_t **func_ptr;
	
	func_status = _get_function_status();
	if (*func_status == SC_FUNC_SET) {
		func_ptr = _get_function_address();
		*func_def = sc_copy_deque(*func_ptr);
		if (*func_def == NULL) {
			err_status = SC_BAD_ALLOC;
		}
	} else {
		err_status = SC_BAD_FUNC;
	}

	return (err_status);
}










