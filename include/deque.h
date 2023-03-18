#ifndef _SMARTCALC_DEQUE_H
#define _SMARTCALC_DEQUE_H

enum TOKEN {BAD_TOKEN = -1, OPERAND, VAR, UNARY_OP, BINARY_OP, LBRACKET, RBRACKET};

enum UNARY_OPERATOR {MINUS, PLUS, COS, SIN, TAN, ACOS, ASIN, ATAN, SQRT, LN, LOG};

enum BINARY_OPERATOR {ADD, SUB, MULT, DIV, MOD, POW};

typedef union u_content {
	double operand;
	enum UNARY_OPERATOR unary_op;
	enum BINARY_OPERATOR binary_op;
} content_t;

typedef struct s_data {
	int type;
	content_t content;
} data_t;

typedef struct node {
	struct node *prev;
	struct node *next;
	data_t data;
} node_t;

typedef struct deque {
	node_t *first;
	node_t *last;
	int (*push_back)(struct deque *, data_t *);
	int (*push_front)(struct deque *, data_t *);
	data_t (*pop_back)(struct deque *);
	data_t (*pop_front)(struct deque *);
	int (*is_empty)(struct deque *);
	void (*clear)(struct deque *);
	data_t * (*peek_back)(struct deque *);
	data_t * (*peek_front)(struct deque *);
} deque_t;

int init_deque(deque_t **deq);

int deque_push_back(deque_t *this, data_t *data);

int deque_push_front(deque_t *this, data_t *data);

data_t deque_pop_back(deque_t *this);

data_t deque_pop_front(deque_t *this);

int deque_is_empty(deque_t *this);

void deque_clear(deque_t *this);

data_t *deque_peek_back(deque_t *this);

data_t *deque_peek_front(deque_t *this);

void deque_print(deque_t *this);

#endif
