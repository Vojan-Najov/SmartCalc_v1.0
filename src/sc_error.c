#include "sc_error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Utilitary function, which print error message in stderr. */

int sc_print_error(const char *message) {
	const char *programm_name = "SmartCalc_v1.0";

	fprintf(stderr, "%s: %s\n", programm_name, message);

	return (SC_FAILURE);
}

/* Functions for handling errors of command line arguments. */

int sc_print_usage(void) {
	fprintf(stderr, SC_HELP_MESSAGE);

	return (SC_FAILURE);
}

int sc_error_program_arg(const char *arg) {
	char buf[128] = "unknown option: ";
	size_t n = 128 - sizeof("unknown option: ") - 1;

	strncat(buf, arg, n);
	sc_print_error(buf);
	sc_print_usage();

	return (SC_FAILURE);
}

/* Functions for handling errors of input reading from terminal. */

void sc_error_input(void *line) {
	free(line);
	sc_print_error(SC_STDIN_ERROR_MESSAGE);
	exit(EXIT_FAILURE);
}

void sc_error_input_alloc(void) {
	sc_print_error(SC_BAD_ALLOC_MESSAGE);
	exit(EXIT_FAILURE);
}

/* Functions for handling lexer's errors. */

void sc_cli_error_lexer(int err_status, sc_deque_t *lexems) {
	lexems->clear(lexems);

	if (err_status == SC_BAD_ALLOC) {
		sc_print_error(SC_BAD_ALLOC_MESSAGE);
		exit(EXIT_FAILURE);
	} else if (err_status == SC_BAD_TOKEN) {
		sc_print_error(SC_BAD_TOKEN_MESSAGE);
	} else if (err_status == SC_BAD_EXPR) {
		sc_print_error(SC_BAD_EXPR_MESSAGE);
	} else if (err_status == SC_BAD_FUNCDEF) {
		sc_print_error(SC_BAD_FUNCDEF_MESSAGE);
	}
}

char *sc_gui_error_lexer(int err_status, sc_deque_t *lexems) {
	char *err_str = NULL;

	lexems->clear(lexems);

	if (err_status == SC_BAD_TOKEN) {
		err_str = strdup(SC_BAD_TOKEN_MESSAGE);
		if (err_str == NULL) {
			err_status = SC_BAD_ALLOC;
		}
	} else if (err_status == SC_BAD_EXPR) {
		err_str = strdup(SC_BAD_EXPR_MESSAGE);
		if (err_str == NULL) {
			err_status = SC_BAD_ALLOC;
		}
	} else if (err_status == SC_BAD_FUNCDEF) {
		err_str = strdup(SC_BAD_FUNCDEF_MESSAGE);
		if (err_str == NULL) {
			err_status = SC_BAD_ALLOC;
		}
	} else if (err_status == SC_NO_TOKENS) {
		err_str = strdup("");
		if (err_str == NULL) {
			err_status = SC_BAD_ALLOC;
		}
	}

	if (err_status == SC_BAD_ALLOC) {
		sc_print_error(SC_BAD_ALLOC_MESSAGE);
		exit(EXIT_FAILURE);
	}

	return (err_str);
}

/* Functions for handling scanner's errors. */

void sc_error_scanner(sc_deque_t *lexems) {
	lexems->clear(lexems);
	sc_print_error(SC_BAD_ALLOC_MESSAGE);
	exit(EXIT_FAILURE);
}

void sc_cli_error_scanner(sc_deque_t *lexems) {
	lexems->clear(lexems);
	sc_print_error(SC_BAD_EXPR_MESSAGE);
}

char *sc_gui_error_scanner(sc_deque_t *lexems) {
	char *err_str;

	lexems->clear(lexems);
	
	err_str = strdup(SC_BAD_EXPR_MESSAGE);
	if(err_str == NULL) { 
		sc_print_error(SC_BAD_ALLOC_MESSAGE);
		exit(EXIT_FAILURE);
	}

	return (err_str);
}

/* Functions for handling scanner's errors. */

void sc_cli_error_parser(int err_status, sc_deque_t *lexems, sc_deque_t *rpn) {
	lexems->clear(lexems);
	rpn->clear(rpn);

	if (err_status == SC_BAD_ALLOC) {
		sc_print_error(SC_BAD_ALLOC_MESSAGE);
		exit(EXIT_FAILURE);
	} else if (err_status == SC_BAD_EXPR) {
		sc_print_error(SC_BAD_EXPR_MESSAGE);
	} else if (err_status == SC_BAD_FUNCDEF) {
		sc_print_error(SC_BAD_FUNCDEF_MESSAGE);
	} else if (err_status == SC_BAD_BRACKET) {
		sc_print_error(SC_BAD_BRACKETS_MESSAGE);
	}
}

char *sc_gui_error_parser(int err_status, sc_deque_t *lexems, sc_deque_t *rpn) {
	char *err_str = NULL;

	lexems->clear(lexems);
	rpn->clear(rpn);

	if (err_status == SC_BAD_EXPR) {
		err_str = strdup(SC_BAD_EXPR_MESSAGE);
		if (err_str == NULL) {
			err_status = SC_BAD_ALLOC;
		}
	} else if (err_status == SC_BAD_FUNCDEF) {
		err_str = strdup(SC_BAD_FUNCDEF_MESSAGE);
		if (err_str == NULL) {
			err_status = SC_BAD_ALLOC;
		}
	} else if (err_status == SC_BAD_BRACKET) {
		err_str = strdup(SC_BAD_BRACKETS_MESSAGE);
		if (err_str == NULL) {
			err_status = SC_BAD_ALLOC;
		}
	}

	if (err_status == SC_BAD_ALLOC) {
		sc_print_error(SC_BAD_ALLOC_MESSAGE);
		exit(EXIT_FAILURE);
	}

	return (err_str);
}

/* Functions for handling calculation's errors. */

void sc_cli_error_calculator(int err_status, sc_deque_t *rpn) {
	rpn->clear(rpn);

	if (err_status == SC_BAD_ALLOC) {
		sc_print_error(SC_BAD_ALLOC_MESSAGE);
		exit(EXIT_FAILURE);
	} else if (err_status == SC_BAD_EXPR) {
		sc_print_error(SC_BAD_EXPR_MESSAGE);
	} else if (err_status == SC_BAD_VAR) {
		sc_print_error(SC_BAD_VAR_MESSAGE);
	} else if (err_status == SC_BAD_FUNC) {
		sc_print_error(SC_BAD_FUNC_MESSAGE);
	} else if (err_status == SC_RECURSIVE_FUNC) {
		sc_print_error(SC_BAD_RECURSIVE_MESSAGE);
	} else if (err_status == SC_DEVIDE_BY_ZERO) {
		sc_print_error(SC_DEVIDE_BY_ZERO_MESSAGE);
	}
}

char *sc_gui_error_calculator(int err_status, sc_deque_t *rpn) {
	char *err_str = NULL;

	rpn->clear(rpn);

	if (err_status == SC_BAD_EXPR) {
		err_str = strdup(SC_BAD_EXPR_MESSAGE);
		if (err_str == NULL) {
			err_status = SC_BAD_ALLOC;
		}
	} else if (err_status == SC_BAD_VAR) {
		err_str = strdup(SC_BAD_VAR_MESSAGE);
		if (err_str == NULL) {
			err_status = SC_BAD_ALLOC;
		}
	} else if (err_status == SC_BAD_FUNC) {
		err_str = strdup(SC_BAD_FUNC_MESSAGE);
		if (err_str == NULL) {
			err_status = SC_BAD_ALLOC;
		}
	} else if (err_status == SC_RECURSIVE_FUNC) {
		err_str = strdup(SC_BAD_RECURSIVE_MESSAGE);
		if (err_str == NULL) {
			err_status = SC_BAD_ALLOC;
		}
	} else if (err_status == SC_DEVIDE_BY_ZERO) {
		err_str = strdup(SC_DEVIDE_BY_ZERO_MESSAGE);
		if (err_str == NULL) {
			err_status = SC_BAD_ALLOC;
		}
	}

	if (err_status == SC_BAD_ALLOC) {
		sc_print_error(SC_BAD_ALLOC_MESSAGE);
		exit(EXIT_FAILURE);
	}

	return (err_str);
}


