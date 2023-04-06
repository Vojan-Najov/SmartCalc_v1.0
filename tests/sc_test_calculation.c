#include "sc_test.h"
#include <check.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

START_TEST(test_01) {
	char buf[512];
	const char *in = "1 + 2\n";
	double result = 1 + 2;
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_02) {
	char buf[512];
	const char *in = "90.871231 - 218218.2121\n";
	double result = 90.871231 - 218218.2121;
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_03) {
	char buf[512];
	const char *in = "90.871231 * 218218.2121\n";
	double result = 90.871231 * 218218.2121;
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_04) {
	char buf[512];
	const char *in = "9084741637.871231 / 218218.2121\n";
	double result = 9084741637.871231 / 218218.2121;
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_05) {
	char buf[512];
	const char *in = "371.12 ^ 2.34";
	double result = pow(371.12, 2.34);
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_06) {
	char buf[512];
	const char *in = "371.12 mod 2.34";
	double result = fmod(371.12, 2.34);
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_07) {
	char buf[512];
	const char *in = "sin(271.38)";
	double result = sin(271.38);
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_08) {
	char buf[512];
	const char *in = "cos(271.38)";
	double result = cos(271.38);
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_09) {
	char buf[512];
	const char *in = "tan(271.38)";
	double result = tan(271.38);
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_10) {
	char buf[512];
	const char *in = "asin(0.98)";
	double result = asin(0.98);
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_11) {
	char buf[512];
	const char *in = "acos(0.98)";
	double result = acos(0.98);
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_12) {
	char buf[512];
	const char *in = "atan(0.98)";
	double result = atan(0.98);
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_13) {
	char buf[512];
	const char *in = "ln(5647.12)";
	double result = log(5647.12);
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_14) {
	char buf[512];
	const char *in = "log(17.83)";
	double result = log(17.83)/log(10);
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_15) {
	char buf[512];
	const char *in = "sqrt(1739377.83)";
	double result = sqrt(1739377.83);
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_16) {
	char buf[512];
	const char *in = "-2837.287 + -26616.1271 - +17.4";
	double result = -2837.287 + -26616.1271 - +17.4;
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_17) {
	char buf[] = "exit";
	const char *in = "12 / 0.0\n";
	fprintf(stderr, "  sc_test_calculation: test_17:\n");
	const char *out = get_smartcalc_result(in);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_18) {
	char buf[] = "exit";
	const char *in = "12 mod 0.0\n";
	fprintf(stderr, "  sc_test_calculation: test_18:\n");
	const char *out = get_smartcalc_result(in);
	
	ck_assert_str_eq(out, buf);
}

START_TEST(test_19) {
	char buf[512];
	const char *in = "2 * (1 + 2) - ((1 - 3) / 2)-(5*4 - sin(3)^2 - cos(3)^2)\n";
	double result = 2 * (1 + 2) - ((1 - 3) / 2)-(5*4 - pow(sin(3),2) - pow(cos(3),2));
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_20) {
	char buf[512];
	const char *in = "1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 +" \
					 "11 + 12 + 13 + 14 + 15 + 16 + 17 + 18 + 19 +" \
					 "21 + 22 + 23 + 24 + 25 + 26 + 27 + 28 + 29 +" \
					 "31 + 32 + 33 + 34 + 35 + 36 + 37 + 38 + 39 +" \
					 "41 + 42 + 43 + 44 + 45 + 46 + 47 + 48 + 49 +" \
					 "51 + 52 + 53 + 54 + 55 + 56 + 57 + 58 + 59 +" \
					 "61 + 62 + 63 + 64 + 65 + 66 + 67 + 68 + 69 +" \
					 "71 + 72 + 73 + 74 + 75 + 76 + 77 + 78 + 79 +" \
					 "81 + 82 + 83 + 84 + 85 + 86 + 87 + 88 + 89 +" \
					 "91 + 92 + 93 + 94 + 95 + 96 + 87 + 98 + 99";
	double result = 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 +
					11 + 12 + 13 + 14 + 15 + 16 + 17 + 18 + 19 +
					21 + 22 + 23 + 24 + 25 + 26 + 27 + 28 + 29 +
					31 + 32 + 33 + 34 + 35 + 36 + 37 + 38 + 39 +
					41 + 42 + 43 + 44 + 45 + 46 + 47 + 48 + 49 +
					51 + 52 + 53 + 54 + 55 + 56 + 57 + 58 + 59 +
					61 + 62 + 63 + 64 + 65 + 66 + 67 + 68 + 69 +
					71 + 72 + 73 + 74 + 75 + 76 + 77 + 78 + 79 +
					81 + 82 + 83 + 84 + 85 + 86 + 87 + 88 + 89 +
					91 + 92 + 93 + 94 + 95 + 96 + 87 + 98 + 99; 
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

START_TEST(test_21) {
	char buf[512];
	const char *in = "sin(2.5) * cos(ln(4) * (1 + 2)) / (tan(12) - sqrt(7*8)) - "\
                     "((1 - 3) / 2)-(5*4 - sin(3)^2 - cos(3)^2)\n";
	double result = sin(2.5) * cos(log(4) * (1 + 2)) / (tan(12) - sqrt(7*8)) - \
                    ((1 - 3) / 2)-(5*4 - pow(sin(3),2) - pow(cos(3),2));
	const char *out = get_smartcalc_result(in);

	sprintf(buf, "%.16g", result);
	
	ck_assert_str_eq(out, buf);
}
END_TEST

Suite *test_calculation(void) {
	Suite *s;
	TCase *tc;

	s = suite_create("sc_test_calculation");
	tc = tcase_create("sc_test_calculation");

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
		tcase_add_test(tc, test_19);
		tcase_add_test(tc, test_20);
		tcase_add_test(tc, test_21);
		suite_add_tcase(s, tc);
	}

	return (s);
}

