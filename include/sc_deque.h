#ifndef _SMARTCALC_DEQUE_H
#define _SMARTCALC_DEQUE_H

/*
  The data structure is a double ended queue based on a doubly linked list.

  Supports operations: push_front, push_back, pop_front, pop_back, is_empty,
                       peek_front, peek_back, reverse, clear.

  The list element is a token structure with type and value fields.

  to initialize the deque -- the create_deque() function;
  to copy the deque -- the copy_deque() function;
  for debaging -- print_deque() function;
*/

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

typedef struct sc_token_s {
	int type;
	union {
		double num;
		enum sc_unary_operator unary_op;
		enum sc_binary_operator binary_op;
		enum sc_function func;
		int error;
	} value;
} sc_token_t;

typedef struct sc_node {
	struct sc_node *prev;
	struct sc_node *next;
	sc_token_t token;
} sc_node_t;

typedef struct sc_deque {
	sc_node_t *first;
	sc_node_t *last;
	int (*push_back)(struct sc_deque *, sc_token_t *);
	int (*push_front)(struct sc_deque *, sc_token_t *);
	sc_token_t (*pop_back)(struct sc_deque *);
	sc_token_t (*pop_front)(struct sc_deque *);
	int (*is_empty)(struct sc_deque *);
	void (*clear)(struct sc_deque *);
	sc_token_t *(*peek_back)(struct sc_deque *);
	sc_token_t *(*peek_front)(struct sc_deque *);
	void (*reverse)(struct sc_deque *);
} sc_deque_t;

sc_deque_t *sc_create_deque(void);

void sc_init_deque(sc_deque_t *deq);

sc_deque_t *sc_copy_deque(sc_deque_t *src);

int sc_deque_push_back(sc_deque_t *this, sc_token_t *data);

int sc_deque_push_front(sc_deque_t *this, sc_token_t *data);

sc_token_t sc_deque_pop_back(sc_deque_t *this);

sc_token_t sc_deque_pop_front(sc_deque_t *this);

int sc_deque_is_empty(sc_deque_t *this);

void sc_deque_clear(sc_deque_t *this);

sc_token_t *sc_deque_peek_back(sc_deque_t *this);

sc_token_t *sc_deque_peek_front(sc_deque_t *this);

void sc_deque_reverse(sc_deque_t *this);

 #ifdef SC_DEBUG
void sc_print_token(sc_token_t token);

void sc_print_deque(sc_deque_t *this);
 #endif 

#endif
