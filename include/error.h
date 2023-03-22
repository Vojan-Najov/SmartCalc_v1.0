#ifndef _SMARTCALV_ERROR_H
#define _SMARTCALV_ERROR_H

#include "deque.h"

int error_create_deque(void);

int error_deque_push(deque_t **deq);

int error_bad_token(deque_t **deq);

int error_parser_alloc(deque_t *lexems, deque_t *stack, deque_t **rpn);

int error_parser_bad_bracket(deque_t *lexems, deque_t *stack, deque_t **rpn);

#endif
