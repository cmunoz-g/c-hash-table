NAME       := c-hash-table
CC         := gcc
CFLAGS     := -std=c11 -Wall -Wextra -Werror -g
CPPFLAGS   := -Iinclude
FOLDER     := src
OBJFOLDER  := build

SRCS       := $(wildcard $(FOLDER)/*.c)
OBJS       := $(SRCS:$(FOLDER)/%.c=$(OBJFOLDER)/%.o)

EXDIR      := examples
EXSRC      := $(EXDIR)/basic.c
EXOBJ      := $(OBJFOLDER)/examples/basic.o
EXE        := $(OBJFOLDER)/examples/basic


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

clean:
	@rm -rf $(OBJFOLDER)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
