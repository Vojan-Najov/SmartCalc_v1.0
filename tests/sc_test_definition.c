#include <check.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sc_test.h"

START_TEST(test_01) {
  char buf[512];
  const char *in = "f = 1 + 2\nf(100)\n";
  double result = 1 + 2;
  const char *out = get_smartcalc_result(in);

  sprintf(buf, "%.16g", result);

  ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_02) {
  char buf[512];
  const char *in = "f = x * 90.871231 - 218218.2121\nf(3)\n";
  double result = 3 * 90.871231 - 218218.2121;
  const char *out = get_smartcalc_result(in);

  sprintf(buf, "%.16g", result);

  ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_03) {
  char buf[512];
  const char *in = "f = x - 90.871231 * 218218.2121\nx = 4\nf(x)\n";
  double result = 4 - 90.871231 * 218218.2121;
  const char *out = get_smartcalc_result(in);

  sprintf(buf, "%.16g", result);

  ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_04) {
  char buf[512];
  const char *in = "f = x * 9084741637.871231 / 218218.2121\nf(6)\n";
  double result = 6 * 9084741637.871231 / 218218.2121;
  const char *out = get_smartcalc_result(in);

  sprintf(buf, "%.16g", result);

  ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_05) {
  char buf[512];
  const char *in = "f = 371.12 ^ x\nf(2.34)\n";
  double result = pow(371.12, 2.34);
  const char *out = get_smartcalc_result(in);

  sprintf(buf, "%.16g", result);

  ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_06) {
  char buf[512];
  const char *in = "f = x mod 2.34\nf(371.12)\n";
  double result = fmod(371.12, 2.34);
  const char *out = get_smartcalc_result(in);

  sprintf(buf, "%.16g", result);

  ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_07) {
  char buf[512];
  const char *in = "f = sin(x)\nf(271.38)\n";
  double result = sin(271.38);
  const char *out = get_smartcalc_result(in);

  sprintf(buf, "%.16g", result);

  ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_08) {
  char buf[512];
  const char *in = "f = cos(x)\nf(271.38)\n";
  double result = cos(271.38);
  const char *out = get_smartcalc_result(in);

  sprintf(buf, "%.16g", result);

  ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_09) {
  char buf[512];
  const char *in = "f = 2 * tan(x)\nf(271.38)\n";
  double result = 2 * tan(271.38);
  const char *out = get_smartcalc_result(in);

  sprintf(buf, "%.16g", result);

  ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_10) {
  char buf[512];
  const char *in = "f = 5.0 / 2.0 + asin(x)\nf(0.98)\n";
  double result = 5.0 / 2.0 + asin(0.98);
  const char *out = get_smartcalc_result(in);

  sprintf(buf, "%.16g", result);

  ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_11) {
  char buf[512];
  const char *in = "f = 3 + 7 - acos(x)\nf(0.98)\n";
  double result = 3 + 7 - acos(0.98);
  const char *out = get_smartcalc_result(in);

  sprintf(buf, "%.16g", result);

  ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_12) {
  char buf[512];
  const char *in = "f = 15 - 3 *4 + 2 * atan(x)\nf(0.98)\n";
  double result = 15 - 3 * 4 + 2 * atan(0.98);
  const char *out = get_smartcalc_result(in);

  sprintf(buf, "%.16g", result);

  ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_13) {
  char buf[512];
  const char *in = "f = ln(x)\nx = 5647.12\nf(x)\n";
  double result = log(5647.12);
  const char *out = get_smartcalc_result(in);

  sprintf(buf, "%.16g", result);

  ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_14) {
  char buf[512];
  const char *in = "f = -log(x)\nx = 17.83\nf(x)\n";
  double result = -log(17.83) / log(10);
  const char *out = get_smartcalc_result(in);

  sprintf(buf, "%.16g", result);

  ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_15) {
  char buf[512];
  const char *in = "f = 15 * sqrt(x)\nf(1739377.83)\n";
  double result = 15 * sqrt(1739377.83);
  const char *out = get_smartcalc_result(in);

  sprintf(buf, "%.16g", result);

  ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_16) {
  char buf[512];
  const char *in = "f = -2837.287 + -26616.1271 - +17.4\nf(3)\n";
  double result = -2837.287 + -26616.1271 - +17.4;
  const char *out = get_smartcalc_result(in);

  sprintf(buf, "%.16g", result);

  ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_17) {
  char buf[] = "exit";
  const char *in = "f = x / 0.0\nf(5)\n";
  fprintf(stderr, "  sc_test_definition: test_17:\n");
  const char *out = get_smartcalc_result(in);

  ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_18) {
  char buf[] = "exit";
  const char *in = "f = 12 mod x\nf(0)\n";
  fprintf(stderr, "  sc_test_definition: test_18:\n");
  const char *out = get_smartcalc_result(in);

  ck_assert_str_eq(out, buf);
}

START_TEST(test_19) {
  char buf[] = "exit";
  const char *in = "f = 12 mod x\nf(x)\n";
  fprintf(stderr, "  sc_test_definition: test_19:\n");
  const char *out = get_smartcalc_result(in);

  ck_assert_str_eq(out, buf);
}

START_TEST(test_20) {
  char buf[512];
  const char *in = "f = 1 + 2\nf = 4 + 1\nf(3)\n";
  double result = 4 + 1;
  const char *out = get_smartcalc_result(in);

  sprintf(buf, "%.16g", result);

  ck_assert_str_eq(out, buf);
}
END_TEST

Suite *test_definition(void) {
  Suite *s;
  TCase *tc;

  s = suite_create("sc_test_definition");
  tc = tcase_create("sc_test_definition");

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
    suite_add_tcase(s, tc);
  }

  return (s);
}
