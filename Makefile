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

# objects
OBJ := $(SRC:.c=.o)
OBJ := $(addprefix $(OBJDIR)/,$(OBJ))

# dependencies
DEPS := $(OBJ:.o=.d)
-include $(DEPS)

# rules
.DEFAULT_GOAL := all

all:
	@$(MAKE) libft
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

re:
	@$(MAKE) fclean
	@$(MAKE) all

# This allows $(NAME) to be run using either an absolute, relative or no path.
# You can pass arguments like this: make run ARGS="hello ' to this world ! ' ."
run:
	@echo
	@PATH=".$${PATH:+:$${PATH}}" && $(NAME) $(ARGS)

valrun:
	@echo
	@PATH=".$${PATH:+:$${PATH}}" && valgrind --suppressions=nc.supp --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes $(NAME) $(ARGS)

rerun:
	@$(MAKE) re
	@$(MAKE) run

l leakcheck:
	@$(MAKE) re
	@$(MAKE) valrun

# these targets are not files
.PHONY: all clean fclean re run rerun leakcheck
.PHONY: libft

# keep intermediate (*.h, *.o, *.d, *.a) targets
.SECONDARY:

# delete failed targets
.DELETE_ON_ERROR:
