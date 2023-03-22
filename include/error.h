#ifndef _SMARTCALV_ERROR_H
#define _SMARTCALV_ERROR_H

#include "deque.h"

#define SMARTCALC_BAD_ALLOC 1
#define SMARTCALC_BAD_TOKEN 2
#define SMARTCALC_BAD_BRACKET 3


int error_create_deque(void);

int error_deque_push(deque_t **deq);

int error_bad_token(deque_t **deq);

int error_parser_alloc(deque_t *lexems, deque_t *stack, deque_t **rpn);

int error_parser_bad_bracket(deque_t *lexems, deque_t *stack, deque_t **rpn);

int error_calculate_alloc(deque_t *stack, deque_t *rpn);

int error_calculate_bad_token(deque_t *stack, deque_t *rpn);

#endif
