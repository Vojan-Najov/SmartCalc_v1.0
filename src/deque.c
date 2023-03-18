#include "deque.h"
#include <stdlib.h>

static node_t *create_node(data_t *data) {
	node_t *node;

	node = (node_t *) malloc(sizeof(node_t));
	if (node != NULL) {
		node->prev = NULL;
		node->next = NULL;
		node->data = *data;
	}

	return (node);
}

int init_deque(deque_t **deq) {
	int status = 0;

	*deq = (deque_t *) malloc(sizeof(deque_t));
	if (*deq != NULL) {
		(*deq)->first = NULL;
		(*deq)->last = NULL;
		(*deq)->push_back = &deque_push_back;
		(*deq)->push_front = &deque_push_front;
		(*deq)->pop_back = &deque_pop_back;
		(*deq)->pop_front = &deque_pop_front;
		(*deq)->is_empty = &deque_is_empty;
		(*deq)->clear = &deque_clear;
		(*deq)->peek_back = &deque_peek_back;
		(*deq)->peek_front = &deque_peek_front;
	} else {
		status = 1;
	}

	return (status);
}

int deque_push_back(deque_t *this, data_t *data) {
	node_t *node;
	int status = 0;

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
		status = 1;
	}

	return (status);
}

int deque_push_front(deque_t *this, data_t *data) {
	node_t *node;
	int status = 0;

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
		status = 1;
	}

	return (status);
}

data_t deque_pop_back(deque_t *this) {
	node_t *node;
	data_t data;

	node = this->last;
	if (node->prev != NULL) {
		this->last = node->prev;
		this->last->next = NULL;
	} else {
		this->last = this->first = NULL;
	}
	data = node->data;
	free(node);

	return (data);
}

data_t deque_pop_front(deque_t *this) {
	node_t *node;
	data_t data;

	node = this->first;
	if (node->next != NULL) {
		this->first = node->next;
		this->first->prev = NULL;
	} else {
		this->first = this->last = NULL;
	}
	data = node->data;
	free(node);

	return (data);
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
}

data_t *deque_peek_back(deque_t *this) {
	return (&this->last->data);
}

data_t *deque_peek_front(deque_t *this) {
	return (&this->first->data);
}

#include <stdio.h>
void deque_print(deque_t *this) {
	node_t *node;

	printf("deque's content:\n");
	if (this != NULL) {
		node = this->first;
		while (node != NULL) {
			printf("_");
			if (node->data.type == OPERAND) {
				printf("%f", node->data.content.operand);
			} else if (node->data.type == VAR) {
				printf("x");
			} else if (node->data.type == UNARY_OP) {
				switch (node->data.content.unary_op) {
					case MINUS:
						printf("-");
						break;
					case PLUS:
						printf("+");
						break;
					case COS:
						printf("cos");
						break;
					case SIN:
						printf("sin");
						break;
					case TAN:
						printf("tan");
						break;
					case ACOS:
						printf("acos");
						break;
					case ASIN:
						printf("asin");
						break;
					case ATAN:
						printf("atan");
						break;
					case SQRT:
						printf("sqrt");
						break;
					case LN:
						printf("ln");
						break;
					case LOG:
						printf("log");
						break;
				}
			} else if (node->data.type == BINARY_OP) {
				switch (node->data.content.binary_op) {
					case ADD:
						printf("+");
						break;
					case SUB:
						printf("-");
						break;
					case MULT:
						printf("*");
						break;
					case DIV:
						printf("/");
						break;
					case MOD:
						printf("%");
						break;
					case POW:
						printf("^");
						break;
				}
			} else if (node->data.type == LBRACKET) {
				printf("(");
			} else if (node->data.type == RBRACKET) {
				printf(")");
			} else {
				printf("ERROR");
			}
			printf("_\n");
			node = node->next;
		}
	} else {
		printf("_deque is NULL_\n");
	}
}
