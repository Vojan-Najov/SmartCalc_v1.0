#include "sc_lexer.h"
#include "sc_error.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static void get_token(const char *str, token_t *token, \
		  			  int prev_token_type, const char **endptr);
static void get_symbolic_operator(char c, token_t *token, int prev_token_type);
static const char *get_functional_operator(const char *str, token_t *token);

deque_t *sc_lexer(const char *str) {
	deque_t *lexems;
	token_t token;
	int err_status = 0;

	token.type = EMPTY_TOKEN;
	lexems = create_deque();
	if (lexems == NULL) {
		sc_error_lexer(lexems);
	}
	while (*str && lexems != NULL) {
		get_token(str, &token, token.type, &str);
		if (token.type == BAD_TOKEN) {
			sc_error_lexer_bad_token(&lexems);
		} else if (token.type != EMPTY_TOKEN) {
			err_status = lexems->push_back(lexems, &token);
			if (err_status) {
				sc_error_lexer(lexems);
			}
		}
	}
	if (lexems->is_empty(lexems)) {
		lexems->clear(lexems);
		lexems = NULL;
	}

	return (lexems);
}

static void get_token(const char *str, token_t *token, \
					  int prev_token_type, const char **endptr) {
	char *endtmp;

	while (isspace(*str)) {
		++str;
	}
	if (*str == '\0') {
		token->type = EMPTY_TOKEN;
	} else if (*str == '(' || *str == ')') {
		if (*str == '(') {
			token->type = LBRACKET;
		} else {
			token->type = RBRACKET;
		}
		++str;
	} else if (tolower(*str) == 'x') {
		token->type = VAR;
		++str;
	} else if (isdigit(*str) || (*str == '.' && isdigit(*(str + 1)))) {
		token->type = NUMBER;
		token->value.num = strtod(str, &endtmp);
		str = endtmp;
	} else if (strchr("-+*/:%^=", *str) != NULL) {
		get_symbolic_operator(*str, token, prev_token_type);
		++str;
	} else {
		str = get_functional_operator(str, token);
	}

	*endptr = str;
}

static void get_symbolic_operator(char c, token_t *token, int prev_token_type) {
	token->type = BINARY_OP;
	if (c == '-') {
		if (prev_token_type == NUMBER || \
			prev_token_type == VAR || \
			prev_token_type == RBRACKET) {
			token->value.binary_op = SUB;
		} else {
			token->type = UNARY_OP;
			token->value.unary_op = MINUS;
		}
	} else if (c == '+') {
		if (prev_token_type == NUMBER || \
			prev_token_type == VAR || \
			prev_token_type == RBRACKET) {
			token->value.binary_op = ADD;
		} else {
			token->type = UNARY_OP;
			token->value.unary_op = PLUS;
		}
	} else if (c == '*') {
		token->value.binary_op = MUL;
	} else if (c == '/' || c == ':') {
		token->value.binary_op = DIV;
	} else if (c == '%') {
		token->value.binary_op = MOD;
	} else if (c == '^') {
		token->value.binary_op = POW;
	} else if (c == '=') {
		token->type = ASSIGN;
	} else {
		token->type = BAD_TOKEN;
	}
} 

static const char *get_functional_operator(const char *str, token_t *token) {
	token->type = FUNCTION;
	if (strncmp(str, "sin", 3) == 0) {
		token->value.func = SIN;
		str += 3;
	} else if (strncmp(str, "cos", 3) == 0) {
		token->value.func = COS;
		str += 3;
	} else if (strncmp(str, "tan", 3) == 0) {
		token->value.func = TAN;
		str += 3;
	} else if (strncmp(str, "asin", 4) == 0) {
		token->value.func = ASIN;
		str += 4;
	} else if (strncmp(str, "acos", 4) == 0) {
		token->value.func = ACOS;
		str += 4;
	} else if (strncmp(str, "atan", 4) == 0) {
		token->value.func = ATAN;
		str += 4;
	} else if (strncmp(str, "sqrt", 4) == 0) {
		token->value.func = SQRT;
		str += 4;
	} else if (strncmp(str, "ln", 2) == 0) {
		token->value.func = LN;
		str += 2;
	} else if (strncmp(str, "log", 3) == 0) {
		token->value.func = LOG;
		str += 3;
	} else if (strncmp(str, "mod", 3) == 0) {
		token->type = BINARY_OP;
		token->value.binary_op = MOD;
		str += 3;
	} else if (tolower(*str) == 'f') {
		token->value.func = F;
		str += 1;
	} else {
		token->type = BAD_TOKEN;
	}

	return (str);
}
