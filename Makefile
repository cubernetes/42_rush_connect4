# TODO: adapt for MacOS
# Makefile for [ "$(uname -s)" = "Linux" ]

# config
NAME := connect4
SRCDIR := .
OBJDIR := obj
LIBFT_DIR := libft

# libs
LIBFT_FILE := libft.a
LIFBT_LIB := $(patsubst lib%,%,$(patsubst %.a,%,$(LIBFT_FILE)))

# tools
CC := cc
RM := /bin/rm -f
MKDIR := /bin/mkdir -p
NM := nm

# flags
CFLAGS := -O2
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Werror
CFLAGS += -pedantic
CFLAGS += -Wconversion
CFLAGS += -Wunreachable-code
CFLAGS += -Wshadow
CFLAGS := -std=c89

CPPFLAGS :=
CPPFLAGS += -MD
CPPFLAGS += -I$(LIBFT_DIR)
CPPFLAGS += -I$(SRCDIR)

LDFLAGS :=
LDFLAGS += -L$(LIBFT_DIR)

LDLIBS :=
LDLIBS += -lncursesw
LDLIBS += -l$(LIFBT_LIB)

# DEBUG=1 make re # include debugging information in the binary
ifeq ($(DEBUG), 1)
	CFLAGS += -ggdb3 -O0
	LDFLAGS += -ggdb3 -O0
endif

# DEBUG=1 make re # include debugging information in the binary
ifeq ($(ASAN), 1)
	CFLAGS += -fsanitize=address
	LDFLAGS += -fsanitize=address
endif

# TSAN=1 make re # check for thread errors and data races
ifeq ($(TSAN), 1)
	CFLAGS += -fsanitize=thread
	LDFLAGS += -fsanitize=thread
endif

# sources
SRC :=

vpath %.c src
SRC += timo-main.c
SRC += main.c
SRC += gameplay.c

# objects
OBJ := $(SRC:.c=.o)
OBJ := $(addprefix $(OBJDIR)/,$(OBJ))

# dependencies
DEPS := $(OBJ:.o=.d)
-include $(DEPS)

# rules
.DEFAULT_GOAL := all

all: libft
	@$(MAKE) $(NAME)

libft:
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(LIBFT_DIR)/$(LIBFT_FILE) $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS) $(LDLIBS)

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $< -c -o $@ $(CPPFLAGS) $(CFLAGS)

$(OBJDIR):
	$(MKDIR) $@

clean:
	$(MAKE) -C $(LIBFT_DIR) $@
	$(RM) $(OBJ)
	$(RM) $(DEPS)
	$(RM) -r $(OBJDIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) $@
	$(RM) $(NAME)

re: fclean
	@$(MAKE) all

# This allows $(NAME) to be run using either an absolute, relative or no path.
# You can pass arguments like this: make run ARGS="hello ' to this world ! ' ."
run:
	@printf '\n'
	@PATH=".$${PATH:+:$${PATH}}" && $(NAME) $(ARGS)

valrun:
	@printf '\n'
	@PATH=".$${PATH:+:$${PATH}}" && valgrind --suppressions=nc.supp --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes $(NAME) $(ARGS)

rerun: re
	@$(MAKE) run

l leakcheck: re
	@$(MAKE) valrun

# - memset can be ignored from nm.
# - write and read are from libft.
# - malloc, free, rand, srand, time are from mandatory part
# - noecho, keypad, initscr, ..., set_escdelay are from ncursesw
# - setlocale is technically forbidden since it's not from ncurses*, but
#   there's otherwise no way to make UTF8 work! so it's needed by us.
# - __* are added by cc
forbidden-funcs-internal:
	@printf '\n'
	@$(NM) -u $(NAME)      | \
		grep -v ' memset@'    | \
		grep -v ' read@'         | \
		grep -v ' write@'        | \
		grep -v ' time@'             | \
		grep -v ' rand@'             | \
		grep -v ' srand@'            | \
		grep -v ' free@'             | \
		grep -v ' malloc@'           | \
		grep -v ' noecho@'          | \
		grep -v ' keypad@'          | \
		grep -v ' cbreak@'          | \
		grep -v ' endwin@'          | \
		grep -v ' wclear@'          | \
		grep -v ' wgetch@'          | \
		grep -v ' initscr@'         | \
		grep -v ' wrefresh@'        | \
		grep -v ' curs_set@'        | \
		grep -v ' wattr_on@'        | \
		grep -v ' mvprintw@'        | \
		grep -v ' getmouse@'        | \
		grep -v ' wattr_off@'       | \
		grep -v ' init_pair@'       | \
		grep -v ' mousemask@'       | \
		grep -v ' init_color@'      | \
		grep -v ' start_color@'     | \
		grep -v ' set_escdelay@'    | \
		grep -v ' setlocale@'          | \
		grep -v ' __gmon_start__'         | \
		grep -v ' __errno_location@'      | \
		grep -v ' __libc_start_main@'     && \
		printf '\033[41;30m%s\033[m\n' "There are forbidden functions!" || \
		( \
			grep --include='*.[hc]' -R 'memset' | grep -v ft_memset && \
			printf '\033[41;30m%s\033[m\n' "You used memset (forbidden)!" || \
			printf '\033[42;30m%s\033[m\n' "No forbidden functions!" \
		)

f forbidden-funcs: re
	@$(MAKE) forbidden-funcs-internal

fl forbidden-funcs-leakcheck: leakcheck
	@$(MAKE) forbidden-funcs-internal

# these targets are not files
.PHONY: all clean fclean re run rerun leakcheck
.PHONY: libft

# keep intermediate (*.h, *.o, *.d, *.a) targets
.SECONDARY:

# delete failed targets
.DELETE_ON_ERROR:
