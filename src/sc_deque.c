#include "sc_deque.h"
#include "sc_error.h"
#include <stdlib.h>

static node_t *create_node(token_t *token) {
	node_t *node;

	node = (node_t *) malloc(sizeof(node_t));
	if (node != NULL) {
		node->prev = NULL;
		node->next = NULL;
		node->token = *token;
	}

	return (node);
}

deque_t *create_deque(void) {
	deque_t *deq;

	deq = (deque_t *) malloc(sizeof(deque_t));
	if (deq != NULL) {
		deq->first = NULL;
		deq->last = NULL;
		deq->push_back = &deque_push_back;
		deq->push_front = &deque_push_front;
		deq->pop_back = &deque_pop_back;
		deq->pop_front = &deque_pop_front;
		deq->is_empty = &deque_is_empty;
		deq->clear = &deque_clear;
		deq->peek_back = &deque_peek_back;
		deq->peek_front = &deque_peek_front;
		deq->reverse = &deque_reverse;
	}

	return (deq);
}

deque_t *copy_deque(deque_t *src) {
	deque_t *copy;
	node_t *node;
	int err_status = 0;

	copy = create_deque();
	if (copy != NULL) {
		node = src->first;
		while (!err_status && node != NULL) {
			err_status = deque_push_back(copy, &node->token);
			node = node->next;
		}
		if (err_status) {
			deque_clear(copy);
			copy = NULL;
		}
	}

	return (copy);
}

int deque_push_back(deque_t *this, token_t *data) {
	node_t *node;
	int status = SC_SUCCESS;

	node = create_node(data);
	if (node != NULL) {
		if (this->last == NULL) {
			this->first = this->last = node;
		} else {
			this->last->next = node;
			node->prev = this->last;
			this->last = node;
		}
	} else {
		status = SC_BAD_ALLOC;
	}

	return (status);
}

int deque_push_front(deque_t *this, token_t *data) {
	node_t *node;
	int status = SC_SUCCESS;

	node = create_node(data);
	if (node != NULL) {
		if (this->first == NULL) {
			this->first = this->last = node;
		} else {
			this->first->prev = node;
			node->next = this->first;
			this->first = node;
		}
	} else {
		status = SC_BAD_ALLOC;
	}

	return (status);
}

token_t deque_pop_back(deque_t *this) {
	node_t *node;
	token_t token;

	node = this->last;
	if (node->prev != NULL) {
		this->last = node->prev;
		this->last->next = NULL;
	} else {
		this->last = this->first = NULL;
	}
	token = node->token;
	free(node);

	return (token);
}

token_t deque_pop_front(deque_t *this) {
	node_t *node;
	token_t token;

	node = this->first;
	if (node->next != NULL) {
		this->first = node->next;
		this->first->prev = NULL;
	} else {
		this->first = this->last = NULL;
	}
	token = node->token;
	free(node);

	return (token);
}

int deque_is_empty(deque_t *this) {
	return (this->first == NULL);
}

void deque_clear(deque_t *this) {
	node_t *tmp;

	while(this->first != NULL) {
		tmp = this->first;
		this->first = tmp->next;
		free(tmp);
	}
	free(this);
}

token_t *deque_peek_back(deque_t *this) {
	return (&this->last->token);
}

token_t *deque_peek_front(deque_t *this) {
	return (&this->first->token);
}

void deque_reverse(deque_t *this) {
	node_t *node;
	node_t *next;
	node_t *prev;

	node = this->last;
	while (node != NULL) {
		next = node->prev;
		prev = node->next;
		node->next = next;
		node->prev = prev;
		node = node->next;
	}
	node = this->first;
	this->first = this->last;
	this->last = node;
}

#include <stdio.h>
void deque_print(deque_t *this) {
	node_t *node;

	printf("deque's value:\n");
	node = this->first;
	while (node != NULL) {
		printf("_");
		token_t token = node->token;
		print_token(token);
		printf("_\n");
		node = node->next;
	}
}

void print_token(token_t token) {
	if (token.type == SC_NUMBER) {
		printf("%f", token.value.num);
	} else if (token.type == SC_VAR) {
		printf("x");
	} else if (token.type == SC_UNARY_OP) {
		switch (token.value.unary_op) {
			case SC_MINUS:
				printf("-");
				break;
			case SC_PLUS:
				printf("+");
				break;
		}
	} else if (token.type == SC_FUNCTION) {
		switch (token.value.func) {
			case SC_COS:
				printf("cos");
				break;
			case SC_SIN:
				printf("sin");
				break;
			case SC_TAN:
				printf("tan");
				break;
			case SC_ACOS:
				printf("acos");
				break;
			case SC_ASIN:
				printf("asin");
				break;
			case SC_ATAN:
				printf("atan");
				break;
			case SC_SQRT:
				printf("sqrt");
				break;
			case SC_LN:
				printf("ln");
				break;
			case SC_LOG:
				printf("log");
				break;
			case SC_F:
				printf("f");
				break;
			}
	} else if (token.type == SC_BINARY_OP) {
		switch (token.value.binary_op) {
			case SC_ADD:
				printf("++");
				break;
			case SC_SUB:
				printf("--");
				break;
			case SC_MUL:
				printf("*");
				break;
			case SC_DIV:
				printf("/");
				break;
			case SC_MOD:
				printf("%%");
				break;
			case SC_POW:
				printf("^");
				break;
		}
	} else if (token.type == SC_LBRACKET) {
		printf("(");
	} else if (token.type == SC_RBRACKET) {
		printf(")");
	} else if (token.type == SC_ASSIGN) {
		printf("=");
	} else {
		printf("BAD TOKEN");
	}
}