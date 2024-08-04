/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nc_gameplay.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 17:30:41 by tischmid          #+#    #+#             */
/*   Updated: 2024/08/04 14:30:17 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "connect4.h"
#include "libft.h"

/* TODO: make sure <curses.h> is NOT included */
#include <locale.h> /* forbidden header */
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

void	no_op(void)
{
}

unsigned long long	get_one_sec(void)
{
	time_t						secs1;
	time_t						secs2;
	static unsigned long long	one_sec;

	if (one_sec != 0)
		return (one_sec);
	one_sec = 0;
	secs1 = time(NULL);
	secs2 = time(NULL);
	while (secs2 == secs1)
		secs2 = time(NULL);
	while (time(NULL) == secs2)
		(no_op(), ++one_sec);
	return (one_sec);
}

unsigned int	get_inc(char **envp)
{
	static unsigned int	inc;

	if (envp != NULL)
		inc = *(unsigned int *)envp;
	return (inc++);
}

/* count utf-8 code points in byte stream */
size_t	ft_utf_8_strlen(const char *s)
{
	size_t	count;

	count = 0;
	while (*s)
		count += (*s++ & 0xc0) != 0x80;
	return (count);
}

void	ft_sleep(double time)
{
	unsigned long long	inc;
	unsigned long long	wait;
	unsigned long long	one_sec;

	one_sec = get_one_sec();
	inc = 0;
	wait = (unsigned long long)((double)one_sec * time);
	while (inc < wait)
		(no_op(), ++inc);
}

void	ft_setenv(char *envp[], const char *name, const char *value)
{
	char	**parts;
	char	*equal_value;

	equal_value = ft_strjoin("=", value);
	while (*envp)
	{
		parts = ft_split(*envp, '=');
		if (!ft_strcmp(parts[0], name))
		{
			*envp = ft_strjoin(name, equal_value);
			break ;
		}
		++envp;
	}
}

void	ncurses_init(void)
{
	initscr();
	keypad(stdscr, TRUE);
	noecho();
	cbreak();
	curs_set(0);
	start_color();
	set_escdelay(0);
	init_colors();
	mousemask(ALL_MOUSE_EVENTS, NULL);
}

void	init(char *envp[])
{
	ft_setenv(envp, "TERM", "xterm-256color");
	setlocale(LC_CTYPE, "");
	ncurses_init();
	attron(COLOR_PAIR(15));
	mvprintw(0, 0, "Initializing...");
	refresh();
	attroff(COLOR_PAIR(15));
	get_one_sec();
	attron(COLOR_PAIR(16));
	clear();
	mvprintw(0, 0, "Done!");
	refresh();
	ft_sleep(0.1);
	clear();
	attroff(COLOR_PAIR(16));
}

void	finish(void)
{
	endwin();
}

void	make_move(t_board *board, int column, int who)
{
	int	row;

	if (column >= board->width || column < 0)
		return ;
	row = board->heigth - 1;
	while (row > 0 && board->cells[row][column] != 0)
		--row;
	if (board->cells[row][column] == 0)
		board->cells[row][column] = who;
}

int	handle_input(t_board *board, int key, int cell_dim)
{
	MEVENT		mouse_event;
	int			column;

	if (key == KEY_MOUSE)
	{
		if (getmouse(&mouse_event) == OK)
		{
			mvprintw(0, 0, "(%s, %s)\n", ft_itoa(mouse_event.x),
				ft_itoa(mouse_event.y));
			column = (mouse_event.x
					- (board->x + board->w / 2
						- board->width * cell_dim * FONT_ASPECT_RATIO / 2))
				/ (FONT_ASPECT_RATIO * cell_dim);
			make_move(board, column, PLAYER_MOVE);
		}
	}
	else
	{
		/* currently, only mouse is supported. */
		/* I know, kinda ironic given this is a ncurses program */
	}
	return (TRUE);
}

int	take_turn(t_board *board, int cell_dim, int *ask_for_input)
{
	int		key;

	if (*ask_for_input)
	{
		key = getch();
		if (!key || key == 'q' || key == '\x1b' || key == '\x04')
			return (FALSE);
		else if (key == KEY_RESIZE)
		{
			board->move_failed = FALSE;
			clear();
			*ask_for_input = !*ask_for_input;
			return (TRUE);
		}
		handle_input(board, key, cell_dim);
	}
	else
		ai_move(board);
	return (TRUE);
}

/* TODO: Implement */
int	random_boolean(void)
{
	return (TRUE);
}

void	nc_gameplay(t_board *board, int no_ai)
{
	int		cell_dim;
	int		ask_for_input;

	ask_for_input = random_boolean();
	(void)no_ai;
	board = init_board(board->heigth, board->width, FALSE);
	while (1)
	{
		print_nc_board(board, 0, 9, COLS, LINES - 9, FALSE, &cell_dim);
		refresh();
		refresh();
		if (no_ai)
			ask_for_input = TRUE;
		if (!take_turn(board, cell_dim, &ask_for_input))
			break ;
		if (FALSE)
		{
			/* print_game_over(board, key); */
		}
		ask_for_input = !ask_for_input;
	}
}
