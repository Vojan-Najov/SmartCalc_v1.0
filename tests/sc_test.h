#ifndef __SC_TEST_H__
#define __SC_TEST_H__

#include <check.h>

#define BSIZE 512

const char *get_smartcalc_result(const char *str);

Suite *test_calculation(void);

Suite *test_assignment(void);

Suite *test_definition(void);

Suite *test_error(void);

#endif
