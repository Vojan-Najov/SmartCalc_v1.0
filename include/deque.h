#ifndef _SMARTCALC_DEQUE_H
#define _SMARTCALC_DEQUE_H

enum TOKEN {BAD_TOKEN = -1, EMPTY_TOKEN = 0, NUMBER, VAR, \
			UNARY_OP, BINARY_OP, FUNCTION, LBRACKET, RBRACKET};
enum UNARY_OPERATOR {MINUS = 0, PLUS = 1};
enum BINARY_OPERATOR {ADD = 0, SUB = 1, MUL = 2, DIV = 3, MOD = 4, POW = 5};
enum FUNCTION {COS = 0, SIN = 1, TAN = 2, ACOS = 3, ASIN = 4, \
			   ATAN = 5, SQRT = 6, LN = 7, LOG = 8};

typedef struct token_s {
	int type;
	union {
		double num;
		enum UNARY_OPERATOR unary_op;
		enum BINARY_OPERATOR binary_op;
		enum FUNCTION func;
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
} deque_t;

deque_t *create_deque(void);

int deque_push_back(deque_t *this, token_t *data);

int deque_push_front(deque_t *this, token_t *data);

token_t deque_pop_back(deque_t *this);

token_t deque_pop_front(deque_t *this);

int deque_is_empty(deque_t *this);

void deque_clear(deque_t *this);

token_t *deque_peek_back(deque_t *this);

token_t *deque_peek_front(deque_t *this);

void print_token(token_t token);

void deque_print(deque_t *this);

#endif
