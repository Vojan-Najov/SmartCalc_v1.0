#include "sc_test.h"
#include <check.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

static int run_test_suite(Suite *test_suite);

int main(void) {
	int number_failed = 0;

	Suite *suite_array[] = {
		test_calculation(),
		test_assignment(),
		test_definition(),
		test_error(),
		NULL
	};

	for (size_t i = 0; suite_array[i] != NULL; ++i) {
		number_failed += run_test_suite(suite_array[i]);
	}

	return (number_failed ? EXIT_FAILURE : EXIT_SUCCESS);
}

static int run_test_suite(Suite *test_suite) {
	int number_failed = 0;
	SRunner *sr;

	sr = srunner_create(test_suite);
	srunner_set_fork_status (sr, CK_NOFORK);
	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);

	return (number_failed);
}

const char *get_smartcalc_result(const char *str) {
	int fd1[2], fd2[2];
	static char buf[BSIZE];
	ssize_t nbytes;
	int status;
	char *args[] = { "tmp", "-t", NULL};
	char *env[] = {NULL};
	char *nl_ptr, *ret = NULL;

	status = pipe(fd1);
	if (status == -1) {
		perror("pipe error");
		exit(EXIT_FAILURE);
	}
	status = pipe(fd2);
	if (status == -1) {
		perror("pipe error");
		exit(EXIT_FAILURE);
	}

	switch (fork()) {
		case -1:
			perror("fork error");
			exit(EXIT_FAILURE);
			break;
		case 0:
			close(fd1[1]);
			dup2(fd1[0], STDIN_FILENO);
			close(fd1[0]);

			close(fd2[0]);
			dup2(fd2[1], STDOUT_FILENO);
			close(fd2[1]);

			execve("./SmartCalc_v1.0", args, env);
			perror("execve");
			exit(EXIT_FAILURE);
			break;
		default:
			close(fd1[0]);
			close(fd2[1]);

			write(fd1[1], str, strlen(str));
			close(fd1[1]);

			nbytes = read(fd2[0], buf, BSIZE);
			close(fd2[0]);

			if (nbytes == -1) {
				perror("read error");
				exit(EXIT_FAILURE);
			}

			if (nbytes == 0) {
				ret = "";
			} else {
				ret = buf;
			}
			nl_ptr = strchr(ret, '\n');
			if (nl_ptr != NULL) {
				*nl_ptr = '\0';
			}
			break;
	}

	return ret;
}
