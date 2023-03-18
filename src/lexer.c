#include "lexer.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static int get_token(const char *str, content_t *content, const char **endptr);
static int get_symbolic_operator(char c, content_t *content, int prev_type);
static int get_functional_operator(const char *str, content_t *content, const char **endptr);

deque_t *lexer(const char *str) {
	deque_t *tokens;
	data_t data = {0};
	int status = 0;

	status = init_deque(&tokens);
	if (!status) {
		while (*str && data.type != BAD_TOKEN) {
			data.type = get_token(str, &data.content, &str);
			if (data.type != BAD_TOKEN) {
				status = tokens->push_back(tokens, &data);
				if (status) {
					tokens->clear(tokens);
					free(tokens);
					perror("smartcalc");
					exit(EXIT_FAILURE);
				}
			}
		}
		if (data.type == BAD_TOKEN && *str) {
			tokens->clear(tokens);
			free(tokens);
			tokens = NULL;
		}
	}

	return (tokens);
}

static int get_token(const char *str, content_t *content, const char **endptr) {
	enum TOKEN type = BINARY_OP;
	char *endtmp;

	content->operand = 0;
	while (isspace(*str)) {
		++str;
	}
	if (*str == '\0') {
		type = BAD_TOKEN;
	} else if (*str == '(' || *str == ')') {
		if (*str == '(') {
			type = LBRACKET;
		} else {
			type = RBRACKET;
		}
		++str;
	} else if (*str == 'x') {
		type = VAR;
		++str;
	} else if (isdigit(*str) || (*str == '.' && isdigit(*(str + 1)))) {
		type = OPERAND;
		content->operand = strtod(str, &endtmp);
		str = endtmp;
	} else if (strchr("-+*/%^", *str) != NULL) {
		type = get_symbolic_operator(*str, content, type);
		++str;
	} else {
		type = get_functional_operator(str, content, &str);
	}

	*endptr = str;

	return (type);
}

static int get_symbolic_operator(char c, content_t *content, int prev_type) {
	enum TOKEN type = BINARY_OP;

	if (c == '-') {
		if (prev_type == OPERAND || prev_type == VAR || prev_type == RBRACKET) {
			content->binary_op = SUB;
		} else {
			type = UNARY_OP;
			content->unary_op = MINUS;
		}
	} else if (c == '+') {
		if (prev_type == OPERAND || prev_type == VAR || prev_type == RBRACKET) {
			content->binary_op = ADD;
		} else {
			type = UNARY_OP;
			content->unary_op = PLUS;
		}
	} else if (c == '*') {
		content->binary_op = MULT;
	} else if (c == '/') {
		content->binary_op = DIV;
	} else if (c == '%') {
		content->binary_op = MOD;
	} else if (c == '^') {
		content->binary_op = POW;
	} else {
		type = BAD_TOKEN;
	}

	return (type);
} 

static int get_functional_operator(const char *str, content_t *content, const char **endptr) {
	enum TOKEN type = UNARY_OP;

	if (strncmp(str, "sin", 3) == 0) {
		content->unary_op = SIN;
		str += 3;
	} else if (strncmp(str, "cos", 3) == 0) {
		content->unary_op = COS;
		str += 3;
	} else if (strncmp(str, "tan", 3) == 0) {
		content->unary_op = TAN;
		str += 3;
	} else if (strncmp(str, "asin", 4) == 0) {
		content->unary_op = ASIN;
		str += 4;
	} else if (strncmp(str, "acos", 4) == 0) {
		content->unary_op = ACOS;
		str += 4;
	} else if (strncmp(str, "atan", 4) == 0) {
		content->unary_op = ATAN;
		str += 4;
	} else if (strncmp(str, "sqrt", 4) == 0) {
		content->unary_op = SQRT;
		str += 4;
	} else if (strncmp(str, "ln", 2) == 0) {
		content->unary_op = LN;
		str += 2;
	} else if (strncmp(str, "log", 3) == 0) {
		content->unary_op = LOG;
		str += 3;
	} else {
		type = BAD_TOKEN;
	}

	*endptr = str;

	return (type);
}
