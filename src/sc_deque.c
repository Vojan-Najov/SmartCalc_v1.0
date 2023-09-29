#include "sc_deque.h"

#include <stdlib.h>

#include "sc_error.h"

static sc_node_t *sc_create_node(sc_token_t *token) {
  sc_node_t *node;

  node = (sc_node_t *)malloc(sizeof(sc_node_t));
  if (node != NULL) {
    node->prev = NULL;
    node->next = NULL;
    node->token = *token;
  }

  return (node);
}

sc_deque_t *sc_create_deque(void) {
  sc_deque_t *deq;

  deq = (sc_deque_t *)malloc(sizeof(sc_deque_t));
  if (deq != NULL) {
    deq->first = NULL;
    deq->last = NULL;
    deq->push_back = &sc_deque_push_back;
    deq->push_front = &sc_deque_push_front;
    deq->pop_back = &sc_deque_pop_back;
    deq->pop_front = &sc_deque_pop_front;
    deq->is_empty = &sc_deque_is_empty;
    deq->clear = &sc_deque_clear;
    deq->peek_back = &sc_deque_peek_back;
    deq->peek_front = &sc_deque_peek_front;
    deq->reverse = &sc_deque_reverse;
  }

  return (deq);
}

void sc_init_deque(sc_deque_t *deq) {
  deq->first = NULL;
  deq->last = NULL;
  deq->push_back = &sc_deque_push_back;
  deq->push_front = &sc_deque_push_front;
  deq->pop_back = &sc_deque_pop_back;
  deq->pop_front = &sc_deque_pop_front;
  deq->is_empty = &sc_deque_is_empty;
  deq->clear = &sc_deque_clear;
  deq->peek_back = &sc_deque_peek_back;
  deq->peek_front = &sc_deque_peek_front;
  deq->reverse = &sc_deque_reverse;
}

sc_deque_t *sc_copy_deque(sc_deque_t *src) {
  sc_deque_t *copy;
  sc_node_t *node;
  int err_status = 0;

  copy = sc_create_deque();
  if (copy != NULL) {
    node = src->first;
    while (!err_status && node != NULL) {
      err_status = sc_deque_push_back(copy, &node->token);
      node = node->next;
    }
    if (err_status) {
      sc_deque_clear(copy);
      copy = NULL;
    }
  }

  return (copy);
}

int sc_deque_push_back(sc_deque_t *this, sc_token_t *data) {
  sc_node_t *node;
  int status = SC_SUCCESS;

  node = sc_create_node(data);
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

int sc_deque_push_front(sc_deque_t *this, sc_token_t *data) {
  sc_node_t *node;
  int status = SC_SUCCESS;

  node = sc_create_node(data);
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

sc_token_t sc_deque_pop_back(sc_deque_t *this) {
  sc_node_t *node;
  sc_token_t token;

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

sc_token_t sc_deque_pop_front(sc_deque_t *this) {
  sc_node_t *node;
  sc_token_t token;

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

int sc_deque_is_empty(sc_deque_t *this) { return (this->first == NULL); }

void sc_deque_clear(sc_deque_t *this) {
  sc_node_t *tmp;

  while (this->first != NULL) {
    tmp = this->first;
    this->first = tmp->next;
    free(tmp);
  }
  this->first = NULL;
  this->last = NULL;
}

void sc_destroy_deque(sc_deque_t *this) {
  this->clear(this);
  free(this);
}

sc_token_t *sc_deque_peek_back(sc_deque_t *this) {
  return (&this->last->token);
}

sc_token_t *sc_deque_peek_front(sc_deque_t *this) {
  return (&this->first->token);
}

void sc_deque_reverse(sc_deque_t *this) {
  sc_node_t *node;
  sc_node_t *next;
  sc_node_t *prev;

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

#ifdef SC_DEBUG
#include <stdio.h>
void sc_print_deque(sc_deque_t *this) {
  sc_node_t *node;

  printf("deque's value:\n");
  node = this->first;
  while (node != NULL) {
    printf("_");
    sc_token_t token = node->token;
    sc_print_token(token);
    printf("_\n");
    node = node->next;
  }
}

void sc_print_token(sc_token_t token) {
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
#endif
