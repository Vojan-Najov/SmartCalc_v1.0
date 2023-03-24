NAME = SmartCalc_v1.0
#TEST = test
#REPORT = gcov_report

INCLD_DIR = ./include
#TEST_INCLD_DIR = ./tests

SRC_DIR = ./src
#TEST_SRC_DIR = ./tests

OBJ_DIR = $(SRC_DIR)/objs
#TEST_OBJ_DIR = $(TEST_SRC_DIR)/test_objs

#GCOV_DIR = $(SRC_DIR)/gcov_report
#GCOV_OBJ_DIR = $(GCOV_DIR)/objs

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(addprefix $(OBJ_DIR)/, $(notdir $(SRC:.c=.o)))

#TEST_SRC = $(wildcard $(TEST_SRC_DIR)/test*.c)
#TEST_OBJ = $(addprefix $(TEST_OBJ_DIR)/, $(notdir $(TEST_SRC:.c=.o)))

#GCOV_OBJ = $(addprefix $(GCOV_OBJ_DIR)/, $(notdir $(SRC:.c=.o)))

INCLD = $(wildcard $(INCLD_DIR)/*.h)
#TEST_INCLD = $(wildcard $(TEST_INCLD_DIR)/*.h)

LIBS = -lm
TEST_LIBS = -lcheck -lm
#TEST_LIBS = -lcheck -lm -lsubunit

CC = gcc
MKDIR = mkdir -p
RM = rm -f
RMDIR = rm -rf
AR = ar rcs

CFLAGS = -Wall -Wextra -Werror -std=c11 -Wpedantic
#GCOV_FLAGS = -fprofile-arcs -ftest-coverage -g -O0

$(NAME): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LIBS)

all: $(NAME)

#$(TEST): $(NAME) $(TEST_OBJ)
#	$(CC) $(TEST_OBJ) $(TEST_LIBS) -L. $(NAME) -o $@

#$(REPORT): $(GCOV_OBJ) $(TEST_OBJ)
#	$(CC) $(GCOV_FLAGS) $(GCOV_OBJ) $(TEST_OBJ)  \
#		-o $(GCOV_DIR)/test_coverage $(TEST_LIBS)
#	./$(GCOV_DIR)/test_coverage;
#	gcov $(GCOV_OBJ_DIR)/*.gcno;
#	@mv *.gcov $(GCOV_OBJ_DIR);
#	lcov -c -t "s21_string" -o $(GCOV_DIR)/report.info -d $(GCOV_OBJ_DIR) \
#		--rc lcov_branch_coverage=1;
#	genhtml -o $(GCOV_DIR) $(GCOV_DIR)/report.info --rc lcov_branch_coverage=1

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLD)
	@$(MKDIR) $(@D)
	$(CC) $(CFLAGS) -I$(INCLD_DIR) -c $< -o $@

#$(TEST_OBJ_DIR)/%.o: $(TEST_SRC_DIR)/%.c  $(TEST_INCLD)
#	@$(MKDIR) $(@D)
#	$(CC) $(CFLAGS) -I$(INCLD_DIR) -I$(TEST_INCLD_DIR) -c $< -o $@

#$(GCOV_OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLD)
#	@$(MKDIR) $(@D)
#	$(CC) $(CFLAGS) $(GCOV_FLAGS) -I$(INCLD_DIR) -c $< -o $@

clean:
	$(RM) $(OBJ)
	$(RMDIR) $(OBJ_DIR)
	$(RM) $(TEST_OBJ)
	$(RMDIR) $(TEST_OBJ_DIR)
	$(RM) -r $(GCOV_OBJ_DIR)

fclean: clean
	$(RM) $(NAME)
	$(RM) $(TEST)
	$(RM) -r $(GCOV_DIR)

format:
	cp ../materials/linters/.clang-format $(SRC_DIR)/.
	clang-format -i $(SRC) $(TEST_SRC) $(INCLD) $(TEST_INCLD)
	$(RM) $(SRC_DIR)/.clang-format

re: fclean all

.PHONY: all clean fclean re format
