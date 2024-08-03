/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timo-main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 17:30:41 by tischmid          #+#    #+#             */
/*   Updated: 2024/08/04 01:04:28 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "connect4.h"
#include "libft.h"

/* TODO: make sure <curses.h> is NOT included */
#include <locale.h> /* forbidden header */
#include <stdlib.h>
#include <ncurses.h>

void	outputArray(int *arr, int size)
{
	int	i;

	for (i = 0; i < size; i++)
		ft_printf("col[%d] = %d\n", i, arr[i]);
}

void	zeroedColum(int *column, int size)
{
	int	i;

	for (i = 0; i < size; i++)
		column[i] = 0;
}

void	addCurrTimeStampAndLength(t_board *board)
{
	t_li	*curr = board->list;
	board->list_length = 1;

	while (curr && curr->next)
	{
		curr = curr->next;
		board->list_length++;
	}

	t_li	*new = ft_malloc(sizeof(t_li));
	new->next = NULL;
	new->time = (unsigned int) time(NULL);
	if (curr == NULL)
		board->list = new;
	else
		curr->next = new;
}

void	trimArrZeros(int *arr, int size)
{
	int	*temp = ft_malloc(sizeof(int) * (size_t)size);
	int	in = 0;
	int	i;

	zeroedColum(temp, size);
	for (i = 0; i < size; i++)
	{
		if (arr[i] != 0)
		{
			temp[in] = arr[i];
			in++;
		}
	}
	for (i = 0; i < size; i++)
		arr[i] = temp[i];
}

void	doubleItUp(int *column, t_board *board)
{
	int	i = 0;

	while (i + 1 < board->height)
	{
		if (column[i] == column[i +1])
		{
			column[i] = column[i] + column[i];
			column[i+1] = 0;
			trimArrZeros(column, board->width);
		}
		i++;
	}
}

void	launch_left(t_board* board)
{
	int	*column = ft_malloc(sizeof(int) * (size_t)board->height);
	int	in;
	int	x;
	int	y;

	for (x = 0; x < board->height; x++)
	{
		in = 0;
		zeroedColum(column, board->width);
		for (y = 0; y < board->width; y++)
		{
			if (board->cells[x][y] != 0)
			{
				column[in] = board->cells[x][y];
				in++;
			}
		}
		doubleItUp(column, board);
		// update the grid
		for (y = 0; y < board->width; y++)
			board->cells[x][y] = column[y];
	}
}

void	launch_right(t_board* board)
{
	int	*column = ft_malloc(sizeof(int) * (size_t)board->height);
	int	in;
	int	x;
	int	y;

	for (x = 0; x < board->height; x++)
	{
		in = 0;
		zeroedColum(column, board->width);
		for (y = board->width -1; y >= 0; y--)
		{
			if (board->cells[x][y] != 0)
			{
				column[in] = board->cells[x][y];
				in++;
			}
		}
		doubleItUp(column, board);
		in = 0;
		for (y = board->width -1; y >= 0; y--, in++)
			board->cells[x][y] = column[in];
	}
}

void	launch_up(t_board* board)
{
	int	*column = ft_malloc(sizeof(int) * (size_t)board->height);
	int	in;
	int	x;
	int	y;

	for (y = 0; y < board->height; y++)
	{
		in = 0;
		zeroedColum(column, board->width);
		for (x = 0; x < board->width; x++)
		{
			if (board->cells[x][y] != 0)
			{
				column[in] = board->cells[x][y];
				in++;
			}
		}
		doubleItUp(column, board);
		// update the grid
		for (x = 0; x < board->width; x++)
			board->cells[x][y] = column[x];
	}
}

void	launch_down(t_board* board)
{
	int	*column = ft_malloc(sizeof(int) * (size_t)board->height);
	int	in;
	int	x;
	int	y;

	for (y = 0; y < board->height; y++)
	{
		in = 0;
		zeroedColum(column, board->width);
		for (x = board->width -1; x >= 0; x--)
		{
			if (board->cells[x][y] != 0)
			{
				column[in] = board->cells[x][y];
				in++;
			}
		}
		doubleItUp(column, board);
		in = 0;
		for (x = board->width -1; x >= 0; x--, in++)
			board->cells[x][y] = column[in];
	}
}

void	copyGrid(int **cpy, int **arr, int height, int width)
{
	int	x;
	int	y;

	for (x = 0; x < height; x++)
	{
		for (y = 0; y < width; y++)
			cpy[x][y] = arr[x][y];
	}
}

bool	gridCompare(int **cpy, int **arr, int height, int width)
{
	int	x;
	int	y;

	for (x = 0; x < height; x++)
	{
		for (y = 0; y < width; y++)
		{
			if (cpy[x][y] != arr[x][y])
				return (false);
		}
	}
	return (true);
}

bool	launch_arrows(t_board *board, int key)
{
	bool	ret;
	int		i;
	int		**cpy;

	cpy = ft_malloc(sizeof(int *) * (size_t)board->height);
	for (i = 0; i < board->height; i++)
	{
		cpy[i] = ft_malloc(sizeof(int) * (size_t)board->width);
		zeroedColum(cpy[i], board->width);
	}
	copyGrid(cpy, board->cells, board->height, board->width);
	board->move_failed = false;
	if (key == KEY_UP || key == 'k')
		launch_up(board);
	else if (key == KEY_DOWN || key == 'j')
		launch_down(board);
	else if (key == KEY_RIGHT || key == 'l')
		launch_right(board);
	else if (key == KEY_LEFT || key == 'h')
		launch_left(board);
	else
		ret = false;
	if (gridCompare(cpy, board->cells, board->height, board->width) == true)
	{
		ret = false;
		board->move_failed = true;
	}
	else
		ret = true;
	return (ret);
}

void	resetGrid(t_board *board, int **temp)
{
	copyGrid(board->cells, temp, board->height, board->width);
}

bool	noMovePossible(t_board *board)
{
	int	i;
	int	**temp;

	temp = ft_malloc(sizeof(int *) * (size_t)board->height);
	for (i = 0; i < board->height; i++)
	{
		temp[i] = ft_malloc(sizeof(int) * (size_t)board->width);
		zeroedColum(temp[i], board->width);
	}
	copyGrid(temp, board->cells, board->height, board->width);
	launch_up(board);
	if (gridCompare(temp, board->cells, board->height, board->width) == false)
		return (resetGrid(board, temp), false);
	launch_down(board);
	if (gridCompare(temp, board->cells, board->height, board->width) == false)
		return (resetGrid(board, temp), false);
	launch_right(board);
	if (gridCompare(temp, board->cells, board->height, board->width) == false)
		return (resetGrid(board, temp), false);
	launch_left(board);
	if (gridCompare(temp, board->cells, board->height, board->width) == false)
		return (resetGrid(board, temp), false);
	return (true);
}

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
	my_init_color(10, 187,173,160); /* border */
	my_init_color(11, 205,192,181); /* bg */
	my_init_color(12, 119,110,101); /* 2 fg */
	my_init_color(13, 238,229,219); /* 2 bg */
	my_init_color(14, 119,110,101); /* 4 fg */
	my_init_color(15, 236,224,201); /* 4 bg */
	my_init_color(16, 249,247,243); /* 8 fg */
	my_init_color(17, 242,177,121); /* 8 bg */
	my_init_color(18, 249,247,243); /* 16 fg */
	my_init_color(19, 244,149,98); /* 16 bg */
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

	init_pair(1, 10, 10); /* border */
	init_pair(2, 11, 11); /* bg */
	init_pair(3, 12, 13); /* 2 */
	init_pair(4, 14, 15); /* 4 */
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
}

void	init(char *envp[])
{
	set_allocator(gc_malloc);
	gc_set_context("GAME");
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
	gc_free_all();
	endwin();
}

t_pos	find_first_zero(t_board *board, t_pos start)
{
	t_pos	found;
	int		i;
	int		j;

	i = start.x;
	while (i < board->height)
	{
		if (i == start.x)
			j = start.y;
		else
			j = 0;
		while (j < board->width)
		{
			if (board->cells[i][j] == 0)
				return (found.x = i, found.y = j, found);
			j++;
		}
		i++;
	}
	return (found.x = -1, found.y = -1, found);
}

t_pos	increment_position(t_board *board, t_pos pos)
{
	if (pos.y < board->height -1)
		return (pos.y++, pos);
	pos.x++;
	if (pos.x >= board->width)
		return (pos.x = 0, pos.y = 0, pos);
	pos.y = 0;
	return (pos);
}

void	setZeroAmount(t_board *board)
{
	int	i;
	int	j;

	board->zero_amount = 0;
	for (i = 0; i < board->height; i++)
	{
		for (j = 0; j < board->width; j++)
		{
			if (board->cells[i][j] == 0)
				board->zero_amount++;
		}
	}
}

t_pos	get_random_zero_pos(t_board *board)
{
	t_pos	ret;
	int		random_num;
	int		pos;
	int		i;

	ret.x = 0;
	ret.y = 0;
	srand((unsigned int)time(NULL) + get_inc(NULL));
	random_num = rand();
	setZeroAmount(board);
	if (board->zero_amount <= 0)
		return (ret.x = -1, ret.y = -1, ret);
	pos = random_num % (board->zero_amount) + 1;
	for (i = 0; i < (int)pos; i++)
	{
		ret = find_first_zero(board, ret);
		if (i == (int)pos -1)
			break ;
		ret = increment_position(board, ret);
	}
	return (ret);
}

t_board	*init_board(int height, int width, bool pre_fill)
{
	t_board	*board;
	int		i;
	int		j;
	int		v;

	board = ft_malloc(sizeof(*board));

	board->width = width;
	board->height = height;
	board->cells = ft_malloc(sizeof(*board->cells) * (size_t)board->height);
	i = -1;
	v = 1;
	while (++i < board->height)
	{
		board->cells[i] = ft_malloc(sizeof(*board->cells[i])
				* (size_t)board->width);
		j = -1;
		while (++j < board->width)
		{
			if (pre_fill)
			{
				if (board->width == 4)
					board->cells[i][j] = 32;
				else if (board->width == 5)
					board->cells[i][j] = 16;
				else if (board->width == 6)
					board->cells[i][j] = 8;
				else
					board->cells[i][j] = 4096;
			}
			else
				board->cells[i][j] = 2048;
		}
	}
	board->one_sec = get_one_sec();
	board->new_cell = (t_pos){.x = -1, .y = -1};
	board->move_failed = false;
	board->x = -1;
	board->y = -1;
	board->w = -1;
	board->h = -1;
	board->win_status = LOSING;
	board->list = NULL;
	board->list_length = 1;
	board->first_game_over = true;
	board->div = 1;
	board->prev_cells = ft_malloc(sizeof(int *) * (size_t)board->height);
	for (i = 0; i < board->height; i++)
		board->prev_cells[i] = ft_malloc(sizeof(int) * (size_t)board->width);
	board->selected = false;
	return (board);
}

t_li *cleanup_old_ones(t_li *list)
{
	t_li			*first;
	unsigned int	curr_time;
	t_li			*prev;
	t_li			*temp;

	first = list;
	curr_time = (unsigned int)time(NULL);
	prev = NULL;
	while (list && list->next)
	{
		if (curr_time - list->time > 2)
		{
			temp = list;
			list = list->next;
			if (prev == NULL)
				first = list;
			else
				prev->next = list;
		}
		else
		{
			prev = list;
			list = list->next;
		}
	}
	return (first);
}

t_li	*update_list(t_board *board)
{
	board->list = cleanup_old_ones(board->list);
	addCurrTimeStampAndLength(board);
	if (board->list_length > 10)
		board->div = 10;
	else if (board->list_length > 5)
		board->div = 7;
	else if (board->list_length >= 3)
		board->div = 3.5;
	else if (board->list_length >= 2)
		board->div = 1.8;
	return (board->list);
}

/* ################## BEGIN PRINTING ############ */
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
		return (0);
	mvprintw(y + cell_dim / 2, x + FONT_ASPECT_RATIO * cell_dim / 2 - num_len / 2, "%d", num);
	return (1);
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

int	print_numbers_height_n(int num, int x, int y, int cell_dim, int height, char *numbers)
{
	int		num_len;
	char	**lines;
	int		line_idx = -1;
	char	*number;

	if (cell_dim - 2 < height || num > 2147483647) /* ascii art only until 2^30. 2^31-1 will be rounded to 2^30. */
		return (0);
	number = extract_number(numbers, num);
	num_len = max_width(number);
	if (num_len > FONT_ASPECT_RATIO * cell_dim - 2)
		return (0);
	lines = ft_split(number, '\n');
	while (++line_idx < height)
		mvprintw(y + cell_dim / 2 - height / 2 + line_idx, x + FONT_ASPECT_RATIO * cell_dim / 2 - num_len / 2, "%s", lines[line_idx]);
	return (1);

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
	num_len = (int)ft_strlen(ft_itoa(num));
	ret = 1;
	if (num == 0)
		ret = 0;
	if (print_numbers_height_n(num, x, y, cell_dim, 16, DOH_NUMBERS))
		ret = 0;
	else if (print_numbers_height_n(num, x, y, cell_dim, 8, DOSREBEL_NUMBERS))
		ret = 0;
	else if (print_numbers_height_n(num, x, y, cell_dim, 6, SHADOW_NUMBERS))
		ret = 0;
	else if (print_numbers_height_n(num, x, y, cell_dim, 4, SMALL_NUMBERS))
		ret = 0;
	else if (print_numbers_height_n(num, x, y, cell_dim, 3, MINI_NUMBERS))
		ret = 0;
	else if (print_number_char(num, x, y, cell_dim, num_len))
		ret = 0;
	attroff(COLOR_PAIR(color));
	return (ret);
}

int	print_number_wrapper(t_board *board, int i, int j, int cell_dim)
{
	int	ret;
	int	y;
	int	x;

	y = board->y + i * cell_dim + board->h / 2 - board->height * cell_dim / 2;
	x = board->x + j * FONT_ASPECT_RATIO * cell_dim + board->w / 2 - board->width * cell_dim * FONT_ASPECT_RATIO / 2;
	ret = print_number(y, x, cell_dim, board->cells[i][j]);
	return (ret);
}

void	print_border_wrapper(t_board *board, int i, int j, int cell_dim, int color)
{
	int	y;
	int	x;

	y = board->y + i * cell_dim + board->h / 2 - board->height * cell_dim / 2;
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
		while (++i < board->height)
		{
			j = -1;
			while (++j < board->width)
				print_border_wrapper(board, i, j, cell_dim, 17);
		}
		refresh();
		ft_sleep(0.1);
	}

	i = -1;
	while (++i < board->height)
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
	while (++i < board->height)
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

	int idx = -1;
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

int	print_board(t_board *board, int x, int y, int w, int h, bool show_scores, int *cell_dim)
{
	board->x = x;
	board->y = y;
	board->w = w;
	board->h = h;
	if (board->h * FONT_ASPECT_RATIO > board->w)
		*cell_dim = board->w / (FONT_ASPECT_RATIO * board->width);
	else
		*cell_dim = board->h / board->height;
	print_borders(board, *cell_dim);
	/* if (show_scores) */
		/* print_scores(board); */
	return (print_numbers(board, *cell_dim));
}
/* ################### END PRINTING ############### */

int	handle_input(t_board *board, int key, int cell_dim)
{
	MEVENT		mouse_event;
	int			column;
	static int	i;

	if (key == KEY_MOUSE)
	{
		if (getmouse(&mouse_event) == OK)
		{
			mvprintw(0, 0, "(%s, %s)\n", ft_itoa(mouse_event.x),
				ft_itoa(mouse_event.y));
			column = (mouse_event.x - (board->x + board->w / 2 - board->width * cell_dim * FONT_ASPECT_RATIO / 2)) / (FONT_ASPECT_RATIO * cell_dim);
			board->cells[board->height - 1][column] = 16;
		}
	}
	else
	{
		mvprintw(0, 0, "%s\n", ft_itoa(key));
		board->cells[0][i] = 16;
	}
	++i;
	return (true);
}

void	game_loop(void)
{
	t_board	*board;
	int		key;
	int		dim;
	t_pos	pos1;
	t_pos	pos2;
	int		cell_dim;

	board = init_board(5, 8, false);
	print_board(board, 0, 9, COLS, LINES - 9, false, &cell_dim);
	mousemask(ALL_MOUSE_EVENTS, NULL);
	key = getch();
	while (key)
	{
		if (key == 'q' || key == '\x1b' || key == '\x04')
			break ;
		else if (key == KEY_RESIZE)
		{
			board->move_failed = false;
			clear();
		}
		else if (handle_input(board, key, cell_dim))
		{
			/* whatever */
		}
		else
		{
			/* nothing happened */
		}
		print_board(board, 0, 9, COLS, LINES - 9, false, &cell_dim);
		if (FALSE)
		{
			/* print_game_over(board, key); */
		}
		refresh();
		refresh();
		key = getch();
	}
}

/* TODO: free as often as possible */
int	main(int argc, char *argv[], char *envp[])
{
	init(envp);
	game_loop();
	finish();
	return (EXIT_SUCCESS);
}
