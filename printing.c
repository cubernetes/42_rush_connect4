/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 14:05:44 by tischmid          #+#    #+#             */
/*   Updated: 2024/08/04 18:56:59 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "connect4.h"
#include "libft.h"

#include <stdlib.h>
#include <ncurses.h>

/* 0 <= BRIGHT <= 1000 */
int	my_init_color(short color, int r, int g, int b)
{
	return (init_color(
			color,
			(short)(r * BRIGHT / 255),
		(short)(g * BRIGHT / 255),
		(short)(b * BRIGHT / 255)
	));
}

void	init_colors(void)
{
	// my_init_color(10, 187,173,160); /* border */
	my_init_color(10, 61,80,202); /* border */
	my_init_color(11, 205,192,181); /* bg */
	my_init_color(12, 119,110,101); /* 2 fg */
	my_init_color(13, 238,229,219); /* 2 bg */
	my_init_color(14, 119,110,101); /* 4 fg */
	my_init_color(15, 236,224,201); /* 4 bg */

	// my_init_color(16, 249,247,243); /* 8 fg */
	// my_init_color(17, 242,177,121); /* 8 bg */
	my_init_color(16, 253,225,61); /* 8 fg */
	my_init_color(17, 253,225,61); /* 8 bg */

	// my_init_color(18, 249,247,243); /* 16 fg */
	// my_init_color(19, 244,149,98); /* 16 bg */
	my_init_color(18, 229,64,53); /* 16 fg */
	my_init_color(19, 229,64,53); /* 16 bg */

	my_init_color(20, 249,247,243); /* 32 fg */
	my_init_color(21, 247,125,96); /* 32 bg */
	my_init_color(22, 249,247,243); /* 64 fg */
	my_init_color(23, 246,94,59); /* 64 bg */
	my_init_color(24, 249,247,243); /* 128 fg */
	my_init_color(25, 236,206,115); /* 128 bg */
	my_init_color(26, 249,247,243); /* 256 fg */
	my_init_color(27, 236,205,98); /* 256 bg */
	my_init_color(28, 249,247,243); /* 512 fg */
	my_init_color(29, 237,200,80); /* 512 bg */
	my_init_color(30, 249,247,243); /* 1024 fg */
	my_init_color(31, 236,196,63); /* 1024 bg */
	my_init_color(32, 249,247,243); /* 2048 fg */
	my_init_color(33, 236,195,45); /* 2048 bg */

	// init_pair(1, 10, 10); /* border */
	init_pair(1, 10, 10); /* border */
	init_pair(2, 11, 11); /* bg */
	init_pair(3, 12, 13); /* 2 */
	init_pair(4, 14, 15); /* 4 */
	// init_pair(5, 16, 17); /* 8 */
	init_pair(5, 16, 17); /* 8 */
	init_pair(6, 18, 19); /* 16 */
	init_pair(7, 20, 21); /* 32 */
	init_pair(8, 22, 23); /* 64 */
	init_pair(9, 24, 25); /* 128 */
	init_pair(10, 26, 27); /* 256 */
	init_pair(11, 28, 29); /* 512 */
	init_pair(12, 30, 31); /* 1024 */
	init_pair(13, 32, 33); /* 2048 */
	init_pair(14, COLOR_BLACK, COLOR_CYAN); /* >= 4096 */

	init_pair(15, COLOR_RED, COLOR_BLACK); /* red text */
	init_pair(16, COLOR_GREEN, COLOR_BLACK); /* green text */
	init_pair(17, COLOR_RED, COLOR_RED); /* border; move failed */
	init_pair(18, COLOR_GREEN, COLOR_GREEN); /* border; selected */
}

void	print_border_top(int x, int y, int cell_dim)
{
	int	d;

	mvprintw(y, x, BOX_DRAWING_TOP_LEFT);
	d = 0;
	while (++d < FONT_ASPECT_RATIO * cell_dim - 1)
		mvprintw(y, x + d, BOX_DRAWING_HORIZONTAL);
	mvprintw(y, x + FONT_ASPECT_RATIO * cell_dim - 1, BOX_DRAWING_TOP_RIGHT);
}

void	print_border_middle(int x, int y, int cell_dim)
{
	int	d;

	d = 0;
	while (++d < cell_dim - 1)
	{
		mvprintw(y + d, x, BOX_DRAWING_VERTICAL);
		mvprintw(y + d, x + FONT_ASPECT_RATIO * cell_dim - 1, BOX_DRAWING_VERTICAL);
	}
}

void	print_border_bottom(int x, int y, int cell_dim)
{
	int	d;

	mvprintw(y + cell_dim - 1, x, BOX_DRAWING_BOTTOM_LEFT);
	d = 0;
	while (++d < FONT_ASPECT_RATIO * cell_dim - 1)
		mvprintw(y + cell_dim - 1, x + d, BOX_DRAWING_HORIZONTAL);
	mvprintw(y + cell_dim - 1, x + FONT_ASPECT_RATIO * cell_dim - 1, BOX_DRAWING_BOTTOM_RIGHT);
}

void	print_border(int y, int x, int cell_dim, int color)
{
	attron(COLOR_PAIR(color));
	print_border_top(x, y, cell_dim);
	print_border_middle(x, y, cell_dim);
	print_border_bottom(x, y, cell_dim);
	attroff(COLOR_PAIR(color));
}

int	max_width(char *lines_str)
{
	char	**lines;
	char	**orig_lines;
	int		max_width = 0;

	orig_lines = lines = ft_split(lines_str, '\n');
	while (*lines)
		max_width = ft_max(max_width, (int)ft_utf_8_strlen(*lines++));
	return (max_width);
}

int	print_number_char(int num, int x, int y, int cell_dim, int num_len)
{
	if (num_len > FONT_ASPECT_RATIO * cell_dim - 1 || cell_dim - 2 < 0)
		return (FALSE);
	mvprintw(y + cell_dim / 2, x + FONT_ASPECT_RATIO * cell_dim / 2 - num_len / 2, "%d", num);
	return (TRUE);
}

char	*extract_number(char *numbers, int number)
{
	char	*ret;
	char	**small_numbers;
	char	*tmp;
	int		lb = 1;

	tmp = ft_replace_all(numbers, "NEWLINE\n", "X");
	small_numbers = ft_split(tmp, 'X');
	while (number >>= 1)
		++lb;
	ret = ft_strdup(small_numbers[lb]);
	return (ret);
}

int	print_numbers_heigth_n(int num, int x, int y, int cell_dim, int heigth, char *numbers)
{
	int		num_len;
	char	**lines;
	int		line_idx;
	char	*number;

	line_idx = -1;
	if (cell_dim - 2 < heigth || num > 2147483647) /* ascii art only until 2^30. 2^31-1 will be rounded to 2^30. */
		return (FALSE);
	number = extract_number(numbers, num);
	num_len = max_width(number);
	if (num_len > FONT_ASPECT_RATIO * cell_dim - 2)
		return (FALSE);
	lines = ft_split(number, '\n');
	while (++line_idx < heigth)
		mvprintw(y + cell_dim / 2 - heigth / 2 + line_idx, x + FONT_ASPECT_RATIO * cell_dim / 2 - num_len / 2, "%s", lines[line_idx]);
	return (TRUE);
}

void	fill_inside_cell(int x, int y, int cell_dim)
{
	int	i;
	int	j;

	i = 0;
	while (++i < cell_dim - 1)
	{
		j = 0;
		while (++j < cell_dim * FONT_ASPECT_RATIO - 1)
			mvprintw(y + i, x + j, " ");
	}
}

int	print_number(int y, int x, int cell_dim, int num)
{
	int	num_len;
	int	color;
	int	ret;

	if (num < 2) /* simplify! */
		color = 2;
	else if (num < 4)
		color = 3;
	else if (num < 8)
		color = 4;
	else if (num < 16)
		color = 5;
	else if (num < 32)
		color = 6;
	else if (num < 64)
		color = 7;
	else if (num < 128)
		color = 8;
	else if (num < 256)
		color = 9;
	else if (num < 512)
		color = 10;
	else if (num < 1024)
		color = 11;
	else if (num < 2048)
		color = 12;
	else if (num < 4096)
		color = 13;
	else
		color = 14;
	attron(COLOR_PAIR(color));
	fill_inside_cell(x, y, cell_dim);
	ret = 0;
	if ((false))
	{
		num_len = (int)ft_strlen(ft_itoa(num));
		ret = 1;
		if (num == 0)
			ret = 0;
		if (print_numbers_heigth_n(num, x, y, cell_dim, 16, DOH_NUMBERS))
			ret = 0;
		else if (print_numbers_heigth_n(num, x, y, cell_dim, 8, DOSREBEL_NUMBERS))
			ret = 0;
		else if (print_numbers_heigth_n(num, x, y, cell_dim, 6, SHADOW_NUMBERS))
			ret = 0;
		else if (print_numbers_heigth_n(num, x, y, cell_dim, 4, SMALL_NUMBERS))
			ret = 0;
		else if (print_numbers_heigth_n(num, x, y, cell_dim, 3, MINI_NUMBERS))
			ret = 0;
		else if (print_number_char(num, x, y, cell_dim, num_len))
			ret = 0;
		attroff(COLOR_PAIR(color));
	}
	return (ret);
}

int	print_number_wrapper(t_board *board, int i, int j, int cell_dim)
{
	int	ret;
	int	y;
	int	x;

	y = board->y + i * cell_dim + board->h / 2 - board->heigth * cell_dim / 2;
	x = board->x + j * FONT_ASPECT_RATIO * cell_dim + board->w / 2 - board->width * cell_dim * FONT_ASPECT_RATIO / 2;
	ret = print_number(y, x, cell_dim, board->cells[i][j]);
	return (ret);
}

void	print_border_wrapper(t_board *board, int i, int j, int cell_dim, int color)
{
	int	y;
	int	x;

	y = board->y + i * cell_dim + board->h / 2 - board->heigth * cell_dim / 2;
	x = board->x + j * FONT_ASPECT_RATIO * cell_dim + board->w / 2 - board->width * cell_dim * FONT_ASPECT_RATIO / 2;
	print_border(y, x, cell_dim, color);
}

void	print_tty_too_small(void)
{
	clear();
	mvprintw(0, 0, "Terminal too small!\nResize it or zoom out!");
	refresh();
}

void	print_borders(t_board *board, int cell_dim)
{
	int	i;
	int	j;

	if (board->move_failed)
	{
		i = -1;
		while (++i < board->heigth)
		{
			j = -1;
			while (++j < board->width)
				print_border_wrapper(board, i, j, cell_dim, 17);
		}
		refresh();
		ft_sleep(0.1);
	}
	i = -1;
	while (++i < board->heigth)
	{
		j = -1;
		while (++j < board->width)
			print_border_wrapper(board, i, j, cell_dim, board->selected ? 18 : 1);
	}
}

int	print_numbers(t_board *board, int cell_dim)
{
	int	i;
	int	j;
	int	new_cell_value;

	i = -1;
	while (++i < board->heigth)
	{
		j = -1;
		while (++j < board->width)
		{
			if (board->new_cell.x == i && board->new_cell.y == j)
			{
				new_cell_value = board->cells[i][j];
				board->cells[i][j] = 0;
				print_number_wrapper(board, i, j, cell_dim);
				board->cells[i][j] = new_cell_value;
			}
			else if (print_number_wrapper(board, i, j, cell_dim))
				return (print_tty_too_small(), EXIT_FAILURE);
		}
	}
	if (board->new_cell.x != -1 && board->new_cell.y != -1)
	{
		refresh();
		ft_sleep(0.7 / board->div);
		if (print_number_wrapper(board, board->new_cell.x, board->new_cell.y, cell_dim))
			return (print_tty_too_small(), EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

char	*repeat_string(char *s, int n)
{
	char	*ret;

	ret = ft_strdup("");
	while (n--)
		ret = ft_strjoin(ret, s);
	return (ret);
}

char	*ft_strjoin_2d(char *left, char *right, int spacing)
{
	char	**left_lines;
	char	**right_lines;
	char	*joined;
	int		idx = 0;
	int		left_max_width;
	int		width;

	left_lines = ft_split(left, '\n');
	right_lines = ft_split(right, '\n');
	left_max_width = max_width(left);

	joined = ft_strdup("");
	while (left_lines[idx] && right_lines[idx])
	{
		joined = ft_strjoin(joined, left_lines[idx]);
		width = (int)ft_utf_8_strlen(left_lines[idx]);
		joined = ft_strjoin(joined, repeat_string(" ", left_max_width - width));
		joined = ft_strjoin(joined, repeat_string(" ", spacing));
		joined = ft_strjoin(joined, right_lines[idx]);
		joined = ft_strjoin(joined, "\n");
		++idx;
	}
	while (left_lines[idx])
	{
		joined = ft_strjoin(joined, left_lines[idx]);
		width = (int)ft_utf_8_strlen(left_lines[idx]);
		joined = ft_strjoin(joined, repeat_string(" ", left_max_width - width));
		joined = ft_strjoin(joined, repeat_string(" ", spacing));
		joined = ft_strjoin(joined, "\n");
		++idx;
	}
	while (right_lines[idx])
	{
		joined = ft_strjoin(joined, right_lines[idx]);
		width = (int)ft_utf_8_strlen(right_lines[idx]);
		joined = ft_strjoin(joined, repeat_string(" ", left_max_width - width));
		joined = ft_strjoin(joined, repeat_string(" ", spacing));
		joined = ft_strjoin(joined, "\n");
		++idx;
	}
	return (joined);
}

void	print_scores(t_board *board)
{
	char	**lines;
	int		width;
	char	*text;
	char	*current_score;
	char	*high_score;
	int		idx;

	current_score = extract_number(DOSREBEL_NUMBERS, (int)board->current_score);
	high_score = extract_number(DOSREBEL_NUMBERS, (int)board->high_score);
	text = ft_strjoin_2d(DOSREBEL_SCORE,
			ft_strjoin_2d(current_score,
				ft_strjoin_2d(DOSREBEL_BEST,
					high_score, 2), 10), 2);
	lines = ft_split(text, '\n');
	width = max_width(text);
	if (width > COLS)
	{
		text = ft_strjoin("SCORE: ",
				ft_strjoin(ft_itoa((int)board->current_score),
					ft_strjoin("     BEST: ",
						ft_itoa((int)board->high_score))));
		lines = ft_split(text, '\n');
		width = max_width(text);
	}
	idx = -1;
	if (board->win_status == WINNING)
		attron(COLOR_PAIR(16));
	while (lines[++idx])
		mvprintw(
			idx,
			COLS / 2 - width / 2,
			"%s\n",
			lines[idx]
			);
	if (board->win_status == WINNING)
		attroff(COLOR_PAIR(16));
}

int	print_nc_board(t_board *board, int x, int y, int w, int h, int show_scores, int *cell_dim)
{
	board->x = x;
	board->y = y;
	board->w = w;
	board->h = h;
	if (board->h * FONT_ASPECT_RATIO > board->w)
		*cell_dim = board->w / (FONT_ASPECT_RATIO * board->width);
	else
		*cell_dim = board->h / board->heigth;
	if ((true)) // TODO: Remove
		return (EXIT_SUCCESS);
	print_borders(board, *cell_dim);
	(void)show_scores;
	/* if (show_scores) */
		/* print_scores(board); */
	return (print_numbers(board, *cell_dim));
}
