#include <check.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sc_test.h"

START_TEST(test_01) {
  char buf[] = "exit";
  const char *in = "dakkdf\n";
  fprintf(stderr, "  sc_test_error: test_01:\n");
  const char *out = get_smartcalc_result(in);

  ck_assert_str_eq(out, buf);
}

START_TEST(test_02) {
  char buf[] = "exit";
  const char *in = "12 mod dhd12\n";
  fprintf(stderr, "  sc_test_error: test_02:\n");
  const char *out = get_smartcalc_result(in);

  ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_03) {
  char buf[] = "exit";
  const char *in = "sin = 3\n";
  fprintf(stderr, "  sc_test_error: test_03:\n");
  const char *out = get_smartcalc_result(in);

  ck_assert_str_eq(out, buf);
}

START_TEST(test_04) {
  char buf[] = "exit";
  const char *in = "3 +\n";
  fprintf(stderr, "  sc_test_error: test_04:\n");
  const char *out = get_smartcalc_result(in);

  ck_assert_str_eq(out, buf);
}

START_TEST(test_05) {
  char buf[] = "exit";
  const char *in = "sin 3 + 4\n";
  fprintf(stderr, "  sc_test_error: test_05:\n");
  const char *out = get_smartcalc_result(in);

  ck_assert_str_eq(out, buf);
}

START_TEST(test_06) {
  char buf[] = "exit";
  const char *in = "sin(3 + 4\n";
  fprintf(stderr, "  sc_test_error: test_06:\n");
  const char *out = get_smartcalc_result(in);

  ck_assert_str_eq(out, buf);
}

START_TEST(test_07) {
  char buf[] = "exit";
  const char *in = "f = f(x) + 3\n";
  fprintf(stderr, "  sc_test_error: test_07:\n");
  const char *out = get_smartcalc_result(in);

  ck_assert_str_eq(out, buf);
}

START_TEST(test_08) {
  char buf[] = "exit";
  const char *in = "f(4) + 3\n";
  fprintf(stderr, "  sc_test_error: test_08:\n");
  const char *out = get_smartcalc_result(in);

  ck_assert_str_eq(out, buf);
}

START_TEST(test_09) {
  char buf[] = "exit";
  const char *in = "x + 3\n";
  fprintf(stderr, "  sc_test_error: test_09:\n");
  const char *out = get_smartcalc_result(in);

  ck_assert_str_eq(out, buf);
}

START_TEST(test_10) {
  char buf[] = "exit";
  const char *in = "x 3\n";
  fprintf(stderr, "  sc_test_error: test_10:\n");
  const char *out = get_smartcalc_result(in);

  ck_assert_str_eq(out, buf);
}

START_TEST(test_11) {
  char buf[] = "exit";
  const char *in = "\n";
  const char *out = get_smartcalc_result(in);

  ck_assert_str_eq(out, buf);
}

START_TEST(test_12) {
  char buf[] = "exit";
  const char *in = "cos-\n";
  fprintf(stderr, "  sc_test_error: test_12:\n");
  const char *out = get_smartcalc_result(in);

  ck_assert_str_eq(out, buf);
}

START_TEST(test_13) {
  char buf[] = "exit";
  const char *in = "* 5 - 4\n";
  fprintf(stderr, "  sc_test_error: test_13:\n");
  const char *out = get_smartcalc_result(in);

  ck_assert_str_eq(out, buf);
}

START_TEST(test_14) {
  char buf[] = "exit";
  const char *in = "-4 sin(3)\n";
  fprintf(stderr, "  sc_test_error: test_14:\n");
  const char *out = get_smartcalc_result(in);

  ck_assert_str_eq(out, buf);
}

START_TEST(test_15) {
  char buf[] = "exit";
  const char *in = "sin(3) (\n";
  fprintf(stderr, "  sc_test_error: test_15:\n");
  const char *out = get_smartcalc_result(in);

  ck_assert_str_eq(out, buf);
}

START_TEST(test_16) {
  char buf[] = "exit";
  const char *in = "sin(3 -)\n";
  fprintf(stderr, "  sc_test_error: test_16:\n");
  const char *out = get_smartcalc_result(in);

  ck_assert_str_eq(out, buf);
}

START_TEST(test_17) {
  char buf[] = "exit";
  const char *in = "                \n";
  const char *out = get_smartcalc_result(in);

  ck_assert_str_eq(out, buf);
}

START_TEST(test_18) {
  char buf[] = "";
  const char *in = "exit\n";
  const char *out = get_smartcalc_result(in);

  ck_assert_str_eq(out, buf);
}

START_TEST(test_19) {
  char buf[] = "exit";
  const char *in = "2 * x\n";
  fprintf(stderr, "  sc_test_error: test_19:\n");
  const char *out = get_smartcalc_result(in);

  ck_assert_str_eq(out, buf);
}

START_TEST(test_20) {
  char buf[] = "exit";
  const char *in = "f(4)\n";
  fprintf(stderr, "  sc_test_error: test_20:\n");
  const char *out = get_smartcalc_result(in);

  ck_assert_str_eq(out, buf);
}

START_TEST(test_21) {
  char buf[] = "exit";
  const char *in = "x = 3\nf(x)\n";
  fprintf(stderr, "  sc_test_error: test_21:\n");
  const char *out = get_smartcalc_result(in);

  ck_assert_str_eq(out, buf);
}

Suite *test_error(void) {
  Suite *s;
  TCase *tc;

  s = suite_create("sc_test_error");
  tc = tcase_create("sc_test_error");

  if (s != NULL && tc != NULL) {
    tcase_add_test(tc, test_01);
    tcase_add_test(tc, test_02);
    tcase_add_test(tc, test_03);
    tcase_add_test(tc, test_04);
    tcase_add_test(tc, test_05);
    tcase_add_test(tc, test_06);
    tcase_add_test(tc, test_07);
    tcase_add_test(tc, test_08);
    tcase_add_test(tc, test_09);
    tcase_add_test(tc, test_10);
    tcase_add_test(tc, test_11);
    tcase_add_test(tc, test_12);
    tcase_add_test(tc, test_13);
    tcase_add_test(tc, test_14);
    tcase_add_test(tc, test_15);
    tcase_add_test(tc, test_16);
    tcase_add_test(tc, test_17);
    tcase_add_test(tc, test_18);
    tcase_add_test(tc, test_19);
    tcase_add_test(tc, test_20);
    tcase_add_test(tc, test_21);
    suite_add_tcase(s, tc);
  }

  return (s);
}
