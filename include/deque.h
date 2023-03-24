#ifndef _SMARTCALC_DEQUE_H
#define _SMARTCALC_DEQUE_H

enum sc_token {
	BAD_TOKEN = -1,
	EMPTY_TOKEN = 0,
	NUMBER,
	VAR,
	ASSIGN,
	UNARY_OP,
	BINARY_OP,
	FUNCTION,
	LBRACKET,
	RBRACKET
};
enum sc_unary_operator {
	MINUS = 0,
	PLUS = 1
};
enum sc_binary_operator {
	ADD = 0,
	SUB = 1,
	MUL = 2,
	DIV = 3,
	MOD = 4,
	POW = 5
};
enum sc_function {
	COS = 0, 
	SIN = 1,
	TAN = 2,
	ACOS = 3,
	ASIN = 4,
	ATAN = 5,
	SQRT = 6,
	LN = 7,
	LOG = 8,
	F = 9,
};

typedef struct token_s {
	int type;
	union {
		double num;
		enum sc_unary_operator unary_op;
		enum sc_binary_operator binary_op;
		enum sc_function func;
	} value;
} token_t;

typedef struct node {
	struct node *prev;
	struct node *next;
	token_t token;
} node_t;

typedef struct deque {
	node_t *first;
	node_t *last;
	int (*push_back)(struct deque *, token_t *);
	int (*push_front)(struct deque *, token_t *);
	token_t (*pop_back)(struct deque *);
	token_t (*pop_front)(struct deque *);
	int (*is_empty)(struct deque *);
	void (*clear)(struct deque *);
	token_t *(*peek_back)(struct deque *);
	token_t *(*peek_front)(struct deque *);
	void (*reverse)(struct deque *);
} deque_t;

deque_t *create_deque(void);

deque_t *copy_deque(deque_t *src);

int deque_push_back(deque_t *this, token_t *data);

int deque_push_front(deque_t *this, token_t *data);

token_t deque_pop_back(deque_t *this);

token_t deque_pop_front(deque_t *this);

int deque_is_empty(deque_t *this);

void deque_clear(deque_t *this);

token_t *deque_peek_back(deque_t *this);

token_t *deque_peek_front(deque_t *this);

void deque_reverse(deque_t *this);

void print_token(token_t token);

void deque_print(deque_t *this);

#endif
