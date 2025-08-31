NAME       := c-hash-table
CC         := gcc
CFLAGS     := -std=c11 -Wall -Wextra -Werror -g
CPPFLAGS   := -Iinclude
FOLDER     := src
OBJFOLDER  := build

SRCS       := $(FOLDER)/hash_table.c $(FOLDER)/prime.c $(FOLDER)/xmem.c
OBJS       := $(SRCS:$(FOLDER)/%.c=$(OBJFOLDER)/%.o)

EXDIR      := examples
EXSRC      := $(EXDIR)/basic.c
EXOBJ      := $(OBJFOLDER)/examples/basic.o
EXE        := $(OBJFOLDER)/examples/basic

TESTDIR    := tests
TESTSRCS   := $(TESTDIR)/test_main.c $(TESTDIR)/test_hash_table.c
TESTOBJS   := $(TESTSRCS:$(TESTDIR)/%.c=$(OBJFOLDER)/tests/%.o)
TESTBIN    := $(OBJFOLDER)/tests/tests

all: $(OBJFOLDER)/libhashtable.a $(EXE)

$(OBJFOLDER)/%.o: $(FOLDER)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJFOLDER)/libhashtable.a: $(OBJS)
	@mkdir -p $(dir $@)
	ar rcs $@ $(OBJS)

$(EXOBJ): $(EXSRC)
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(EXE): $(EXOBJ) $(OBJFOLDER)/libhashtable.a
	$(CC) $(CFLAGS) $^ -o $@

$(OBJFOLDER)/tests/%.o: $(TESTDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(TESTBIN): $(TESTOBJS) $(OBJFOLDER)/libhashtable.a
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@

tests: $(TESTBIN)
	valgrind --leak-check=full --error-exitcode=1 ./$(TESTBIN)

run: $(EXE)
	./$(EXE)

clean:
	@rm -rf $(OBJFOLDER)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re tests run
