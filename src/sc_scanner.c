#include "sc_scanner.h"
#include "sc_error.h"

int sc_scanner(deque_t *lexems) {
	int err_status;
	token_t token;
	int status = SC_EXPRESSION;

	token = lexems->pop_front(lexems);
	if (token.type == VAR) {
		if (!lexems->is_empty(lexems)) {
			if (lexems->peek_front(lexems)->type == ASSIGN) {
				lexems->pop_front(lexems);
				status = lexems->is_empty(lexems) ? SC_BAD_EXPR : SC_ASSIGNMENT;
			}
		}
	} else if (token.type == FUNCTION && token.value.func == F) {
		if (!lexems->is_empty(lexems)) {
			if (lexems->peek_front(lexems)->type == ASSIGN) {
				lexems->pop_front(lexems);
				status = lexems->is_empty(lexems) ? SC_BAD_EXPR : SC_DEFINITION;
			}
		}
	}
	if (status == SC_EXPRESSION) {
		err_status = lexems->push_front(lexems, &token);
	} else if (status == SC_BAD_EXPR) {
		lexems->clear(lexems);
	}
	if (err_status == SC_BAD_ALLOC) {
		sc_error_scanner(lexems);
	}

	return (status);
}
