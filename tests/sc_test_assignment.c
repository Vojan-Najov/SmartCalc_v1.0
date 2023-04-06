#include "sc_test.h"
#include <check.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

START_TEST(test_01) {
	char buf[512];
	const char *in = "x = 1 + 2\nx\n";
	double result = 1 + 2;
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_02) {
	char buf[512];
	const char *in = "x = 90.871231 - 218218.2121\nx\n";
	double result = 90.871231 - 218218.2121;
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_03) {
	char buf[512];
	const char *in = "x = 90.871231 * 218218.2121\nx\n";
	double result = 90.871231 * 218218.2121;
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_04) {
	char buf[512];
	const char *in = "x = 9084741637.871231 / 218218.2121\nx\n";
	double result = 9084741637.871231 / 218218.2121;
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_05) {
	char buf[512];
	const char *in = "x = 371.12 ^ 2.34\nx\n";
	double result = pow(371.12, 2.34);
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_06) {
	char buf[512];
	const char *in = "x = 371.12 mod 2.34\nx\n";
	double result = fmod(371.12, 2.34);
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_07) {
	char buf[512];
	const char *in = "x = sin(271.38)\nx\n";
	double result = sin(271.38);
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_08) {
	char buf[512];
	const char *in = "x = cos(271.38)\nx\n";
	double result = cos(271.38);
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_09) {
	char buf[512];
	const char *in = "x = tan(271.38)\nx\n";
	double result = tan(271.38);
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_10) {
	char buf[512];
	const char *in = "x = asin(0.98)\nx\n";
	double result = asin(0.98);
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_11) {
	char buf[512];
	const char *in = "x = acos(0.98)\nx\n";
	double result = acos(0.98);
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_12) {
	char buf[512];
	const char *in = "x = atan(0.98)\nx\n";
	double result = atan(0.98);
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_13) {
	char buf[512];
	const char *in = "x = ln(5647.12)\nx\n";
	double result = log(5647.12);
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_14) {
	char buf[512];
	const char *in = "x = log(17.83)\nx\n";
	double result = log(17.83)/log(10);
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_15) {
	char buf[512];
	const char *in = "x = sqrt(1739377.83)\nx\n";
	double result = sqrt(1739377.83);
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_16) {
	char buf[512];
	const char *in = "x = -2837.287 + -26616.1271 - +17.4\nx\n";
	double result = -2837.287 + -26616.1271 - +17.4;
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_17) {
	char buf[] = "exit";
	const char *in = "x = 12 / 0.0\n";
	fprintf(stderr, "  sc_test_assignment: test_17:\n");
	const char *out = get_smartcalc_result(in);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_18) {
	char buf[] = "exit";
	const char *in = "x = 12 mod 0.0\n";
	fprintf(stderr, "  sc_test_assignment: test_18:\n");
	const char *out = get_smartcalc_result(in);
	
	ck_assert_str_eq(out, buf);
}

Suite *test_assignment(void) {
	Suite *s;
	TCase *tc;

	s = suite_create("sc_test_assignment");
	tc = tcase_create("sc_test_assignment");

	if (s!= NULL && tc != NULL) {
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
		suite_add_tcase(s, tc);
	}

	return (s);
}

