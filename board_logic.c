/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   board_logic.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 14:06:14 by tischmid          #+#    #+#             */
/*   Updated: 2024/08/04 14:12:45 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "connect4.h"
#include "libft.h"

#include <stddef.h>
#include <stdlib.h>
#include <time.h>
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

	while (i + 1 < board->heigth)
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
	int	*column = ft_malloc(sizeof(int) * (size_t)board->heigth);
	int	in;
	int	x;
	int	y;

	for (x = 0; x < board->heigth; x++)
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
	int	*column = ft_malloc(sizeof(int) * (size_t)board->heigth);
	int	in;
	int	x;
	int	y;

	for (x = 0; x < board->heigth; x++)
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
	int	*column = ft_malloc(sizeof(int) * (size_t)board->heigth);
	int	in;
	int	x;
	int	y;

	for (y = 0; y < board->heigth; y++)
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
	int	*column = ft_malloc(sizeof(int) * (size_t)board->heigth);
	int	in;
	int	x;
	int	y;

	for (y = 0; y < board->heigth; y++)
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

void	copyGrid(int **cpy, int **arr, int heigth, int width)
{
	int	x;
	int	y;

	for (x = 0; x < heigth; x++)
	{
		for (y = 0; y < width; y++)
			cpy[x][y] = arr[x][y];
	}
}

int	gridCompare(int **cpy, int **arr, int heigth, int width)
{
	int	x;
	int	y;

	for (x = 0; x < heigth; x++)
	{
		for (y = 0; y < width; y++)
		{
			if (cpy[x][y] != arr[x][y])
				return (FALSE);
		}
	}
	return (TRUE);
}

int	launch_arrows(t_board *board, int key)
{
	int	ret;
	int	i;
	int	**cpy;

	cpy = ft_malloc(sizeof(int *) * (size_t)board->heigth);
	for (i = 0; i < board->heigth; i++)
	{
		cpy[i] = ft_malloc(sizeof(int) * (size_t)board->width);
		zeroedColum(cpy[i], board->width);
	}
	copyGrid(cpy, board->cells, board->heigth, board->width);
	board->move_failed = FALSE;
	if (key == KEY_UP || key == 'k')
		launch_up(board);
	else if (key == KEY_DOWN || key == 'j')
		launch_down(board);
	else if (key == KEY_RIGHT || key == 'l')
		launch_right(board);
	else if (key == KEY_LEFT || key == 'h')
		launch_left(board);
	else
		ret = FALSE;
	if (gridCompare(cpy, board->cells, board->heigth, board->width) == TRUE)
	{
		ret = FALSE;
		board->move_failed = TRUE;
	}
	else
		ret = TRUE;
	return (ret);
}

void	resetGrid(t_board *board, int **temp)
{
	copyGrid(board->cells, temp, board->heigth, board->width);
}

int	noMovePossible(t_board *board)
{
	int	i;
	int	**temp;

	temp = ft_malloc(sizeof(int *) * (size_t)board->heigth);
	for (i = 0; i < board->heigth; i++)
	{
		temp[i] = ft_malloc(sizeof(int) * (size_t)board->width);
		zeroedColum(temp[i], board->width);
	}
	copyGrid(temp, board->cells, board->heigth, board->width);
	launch_up(board);
	if (gridCompare(temp, board->cells, board->heigth, board->width) == FALSE)
		return (resetGrid(board, temp), FALSE);
	launch_down(board);
	if (gridCompare(temp, board->cells, board->heigth, board->width) == FALSE)
		return (resetGrid(board, temp), FALSE);
	launch_right(board);
	if (gridCompare(temp, board->cells, board->heigth, board->width) == FALSE)
		return (resetGrid(board, temp), FALSE);
	launch_left(board);
	if (gridCompare(temp, board->cells, board->heigth, board->width) == FALSE)
		return (resetGrid(board, temp), FALSE);
	return (TRUE);
}


t_pos	find_first_zero(t_board *board, t_pos start)
{
	t_pos	found;
	int		i;
	int		j;

	i = start.x;
	while (i < board->heigth)
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
	if (pos.y < board->heigth -1)
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
	for (i = 0; i < board->heigth; i++)
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

t_board	*init_board(int heigth, int width, int pre_fill)
{
	t_board	*board;
	int		i;
	int		j;
	int		v;

	board = ft_malloc(sizeof(*board));

	board->width = width;
	board->heigth = heigth;
	board->cells = ft_malloc(sizeof(*board->cells) * (size_t)board->heigth);
	i = -1;
	v = 1;
	while (++i < board->heigth)
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
				board->cells[i][j] = 0;
		}
	}
	board->one_sec = get_one_sec();
	board->new_cell = (t_pos){.x = -1, .y = -1};
	board->move_failed = FALSE;
	board->x = -1;
	board->y = -1;
	board->w = -1;
	board->h = -1;
	board->win_status = LOSING;
	board->list = NULL;
	board->list_length = 1;
	board->first_game_over = TRUE;
	board->div = 1;
	board->prev_cells = ft_malloc(sizeof(int *) * (size_t)board->heigth);
	for (i = 0; i < board->heigth; i++)
		board->prev_cells[i] = ft_malloc(sizeof(int) * (size_t)board->width);
	board->selected = FALSE;
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
