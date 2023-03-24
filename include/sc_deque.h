#ifndef _SMARTCALC_DEQUE_H
#define _SMARTCALC_DEQUE_H

enum sc_token {
	SC_WRONG_TOKEN = -1,
	SC_EMPTY_TOKEN = 0,
	SC_ASSIGN,
	SC_NUMBER,
	SC_VAR,
	SC_UNARY_OP,
	SC_BINARY_OP,
	SC_FUNCTION,
	SC_LBRACKET,
	SC_RBRACKET
};
enum sc_unary_operator {
	SC_MINUS = 0,
	SC_PLUS = 1
};
enum sc_binary_operator {
	SC_ADD = 0,
	SC_SUB = 1,
	SC_MUL = 2,
	SC_DIV = 3,
	SC_MOD = 4,
	SC_POW = 5
};
enum sc_function {
	SC_COS = 0, 
	SC_SIN = 1,
	SC_TAN = 2,
	SC_ACOS = 3,
	SC_ASIN = 4,
	SC_ATAN = 5,
	SC_SQRT = 6,
	SC_LN = 7,
	SC_LOG = 8,
	SC_F = 9,
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
