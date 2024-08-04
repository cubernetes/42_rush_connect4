/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkoca <dkoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 00:56:34 by dkoca             #+#    #+#             */
/*   Updated: 2024/08/04 03:22:58 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <stdlib.h>
#include <unistd.h>

#define PLAYER_TURN 1
#define AI_TURN 2
#define WIN 0
#define CONTINUE 1
#define PLAYER_WINS 2
#define AI_WINS 3
#define PLAYER_EOF -1

void	print_board(int **board, int heigth, int width)
{
	int	i;
	int	j;

	ft_printf("Current state of the board:\n");
	for (i = 0; i < heigth; i++)
	{
		for (j = 0; j < width; j++)
			ft_printf(" %i ", board[i][j]);
		ft_printf("\n");
	}
}

int	check_secondary_diagonal(int **board, int player, int heigth, int width)
{
	int	consec;
	int	i;
	int	j;
	int	k;

	for (i = 0; i < heigth - 3; i++)
	{
		for (j = width - 1; j > 2; j--)
		{
			consec = 0;
			for (k = 0; k < 4; k++)
				if (board[i + k][j - k] == player)
					consec++;
			if (consec == 4)
			{
				ft_printf("Player %i is the WINNER!\n", player);
				return (EXIT_SUCCESS);
			}
			ft_printf("i = %i, j = %i\n", i, j);
		}
	}
	return (EXIT_FAILURE);
}

int	check_primary_diagonal(int **board, int player, int heigth, int width)
{
	int	consec;
	int	i;
	int	j;
	int	k;

	for (i = 0; i < heigth - 3; i++)
	{
		for (j = 0; j < width - 3; j++)
		{
			consec = 0;
			for (k = 0; k < 4; k++)
				if (board[i + k][j + k] == player)
					consec++;
			if (consec == 4)
			{
				ft_printf("Player %i is the WINNER!\n", player);
				return (WIN);
			}
			ft_printf("i = %i, j = %i\n", i, j);
		}
	}
	return (CONTINUE);
}

int	check_horizontal(int **board, int player, int heigth, int width)
{
	int	consec;
	int	i;
	int	j;

	for (i = 0; i < heigth; i++)
	{
		consec = 0;
		for (j = 1; j < width; j++)
		{
			if (board[i][j] == player && board[i][j - 1] == player)
				consec++;
			if (consec == 3)
			{
				ft_printf("Player %i is the WINNER!\n", player);
				return (WIN);
			}
		}
	}
	return (CONTINUE);
}

int	check_vertical(int **board, int player, int heigth, int width)
{
	int	consec;
	int	i;
	int	j;

	for (i = 0; i < width; i++)
	{
		consec = 0;
		for (j = 1; j < heigth; j++)
		{
			if (board[j][i] == player && board[j - 1][i] == player)
				consec++;
			if (consec == 3)
			{
				ft_printf("Player %i is the WINNER!\n", player);
				return (WIN);
			}
		}
	}
	return (CONTINUE);
}

int	check_win_states(int **board, int player, int heigth, int width)
{
	if (!check_primary_diagonal(board, player, heigth, width)
		|| !check_secondary_diagonal(board, player, heigth, width)
		|| !check_vertical(board, player, heigth, width)
		|| !check_horizontal(board, player, heigth, width))
		return (WIN);
	return (CONTINUE);
}

int	is_full(int **board, int heigth, int width)
{
	int	i;
	int	j;

	for (i = 0; i < heigth; i++)
	{
		for (j = 0; j < width; j++)
			if (board[i][j] == 0)
				return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

void	check_invalid_move(int **board, int cur_row, int move, int heigth)
{
	char	*input;

	while (!cur_row && board[cur_row][move] != 0)
	{
		ft_printf("Find another move!\n");
		input = get_next_line(STDIN_FILENO);
		ft_printf("The move is:\n");
		ft_printf("%s", input);
		move = ft_atoi(input) - 1;
		cur_row = heigth - 1;
		while (cur_row && board[cur_row][move] != 0)
			cur_row--;
	}
	return ;
}

int	player_turn(int **board, int player, int heigth, int width)
{
	char	*input;
	int		cur_row;
	int		move;

	cur_row = heigth - 1;
	input = get_next_line(0);
	if (!input)
	{
		ft_printf("Bye!\n");
		return (PLAYER_EOF);
	}
	ft_printf("The move is:\n");
	ft_printf("%s", input);
	move = ft_atoi(input) - 1;
	while (cur_row && board[cur_row][move] != 0)
		cur_row--;
	check_invalid_move(board, cur_row, move, heigth);
	if (move < width)
		board[cur_row][move] = player;
	return (move);
}

int	gameplay(int **board, int heigth, int width, int no_ai)
{
	int	move;

	(void)no_ai;
	print_board(board, heigth, width);
	while (!is_full(board, heigth, width))
	{
		ft_printf("Player One's turn:\n");
		move = player_turn(board, PLAYER_TURN, heigth, width);
		if (move == PLAYER_EOF)
			return (EXIT_SUCCESS);
		print_board(board, heigth, width);
		if (!check_win_states(board, PLAYER_TURN, heigth, width))
			return (PLAYER_WINS);
		ft_printf("Player Two's turn:\n");
		move = player_turn(board, AI_TURN, heigth, width);
		if (move == PLAYER_EOF)
			return (EXIT_SUCCESS);
		print_board(board, heigth, width);
		if (!check_win_states(board, AI_TURN, heigth, width))
			return (AI_WINS);
	}
	return (EXIT_FAILURE);
}

int	init_game_board(int ***board, int height, int width)
{
	int	i;

	*board = ft_malloc(sizeof(int *) * (size_t)(height));
	for (i = 0; i < height; i++)
		(*board)[i] = ft_calloc((size_t)(width), sizeof(int));
	return (EXIT_SUCCESS);
}
