NAME = SmartCalc_v1.0
DIST_NAME = SmartCalc-1.0
TEST = test
REPORT = gcov_report

INCLD_DIR = ./include
TEST_INCLD_DIR = ./tests

SRC_DIR = ./src
TEST_SRC_DIR = ./tests

OBJ_DIR = $(SRC_DIR)/objs
TEST_OBJ_DIR = $(TEST_SRC_DIR)/test_objs

GCOV_DIR = ./gcov_report
GCOV_OBJ_DIR = $(GCOV_DIR)/objs

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(addprefix $(OBJ_DIR)/, $(notdir $(SRC:.c=.o)))

TEST_SRC = $(wildcard $(TEST_SRC_DIR)/*test*.c)
TEST_OBJ = $(addprefix $(TEST_OBJ_DIR)/, $(notdir $(TEST_SRC:.c=.o)))

GCOV_OBJ = $(addprefix $(GCOV_OBJ_DIR)/, $(notdir $(SRC:.c=.o)))

INCLD = $(wildcard $(INCLD_DIR)/*.h)
TEST_INCLD = $(wildcard $(TEST_INCLD_DIR)/*.h)

CC = gcc
MKDIR = mkdir -p
RM = rm -f
RMDIR = rm -rf
PKG-CONFIG = $(shell which pkg-config)

ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

CFLAGS = -g -Wall -Wextra -Werror -std=c11 -I$(INCLD_DIR) -D_GNU_SOURCE
GTK_CFLAGS = $(shell $(PKG-CONFIG) --cflags gtk4)
GCOV_FLAGS = -fprofile-arcs -ftest-coverage -g -O0

LIBS = -lm
GTK_LIBS = $(shell $(PKG-CONFIG) --libs gtk4)
TEST_LIBS = -lcheck -lm
#TEST_LIBS += -lsubunit

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -g -o $@ $(OBJ) $(LIBS) $(GTK_LIBS)

install: $(NAME)
	install -d $(DESTDIR)$(PREFIX)/bin/
	install -m 755 $(NAME) $(DESTDIR)$(PREFIX)/bin/

uninstall:
	$(RM) $(DESTDIR)$(PREFIX)/bin/$(NAME)

dist: fclean
	@$(MKDIR) $(DIST_NAME)
	cp -r -f $(SRC_DIR) $(TEST_SRC_DIR) $(INCLD_DIR) README.md Makefile $(DIST_NAME)/
	tar -czvf $(DIST_NAME).tar.gz $(DIST_NAME)
	$(RM) -r $(DIST_NAME)

$(TEST): $(NAME) $(TEST_OBJ)
	$(CC) -g -o $@ $(TEST_OBJ) $(TEST_LIBS)
	./$(TEST) 2>>/dev/null

$(REPORT): $(GCOV_OBJ) $(TEST)
	$(CC) $(GCOV_FLAGS) $(GCOV_OBJ) -o $(NAME) $(LIBS) $(GTK_LIBS)
	./$(TEST) 2>>/dev/null;
	@$(RM) $(GCOV_OBJ_DIR)/sc_gui*
	gcov $(GCOV_OBJ_DIR)/*.gcno;
	@mv *.gcov $(GCOV_OBJ_DIR);
	lcov -c -t "s21_string" -o $(GCOV_DIR)/report.info -d $(GCOV_OBJ_DIR) \
		--rc lcov_branch_coverage=1;
	genhtml -o $(GCOV_DIR) $(GCOV_DIR)/report.info --rc lcov_branch_coverage=1

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLD)
	@$(MKDIR) $(@D)
	$(CC) $(CFLAGS) $(GTK_CFLAGS) -c $< -o $@

$(TEST_OBJ_DIR)/%.o: $(TEST_SRC_DIR)/%.c  $(TEST_INCLD)
	@$(MKDIR) $(@D)
	$(CC) $(CFLAGS) -I$(TEST_INCLD_DIR) -c $< -o $@

$(GCOV_OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLD)
	@$(MKDIR) $(@D)
	$(CC) $(CFLAGS) $(GTK_CFLAGS) $(GCOV_FLAGS) -I$(INCLD_DIR) -c $< -o $@

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
	$(RM) $(DIST_NAME).tar.gz

format:
	cp ../materials/linters/.clang-format $(SRC_DIR)/.
	clang-format -i $(SRC) $(TEST_SRC) $(INCLD) $(TEST_INCLD)
	$(RM) $(SRC_DIR)/.clang-format

check_ui:
	gtk4-builder-tool validate ./resources/test.ui

re: fclean all

.PHONY: all install clean fclean re format check_ui
