/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkoca <dkoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 00:56:34 by dkoca             #+#    #+#             */
/*   Updated: 2024/08/04 03:51:36 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "connect4.h"
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

void	print_board(t_board *board)
{
	int	i;
	int	j;

	ft_printf("Current state of the board:\n");
	for (i = 0; i < board->heigth; i++)
	{
		for (j = 0; j < board->width; j++)
			ft_printf(" %i ", board->cells[i][j]);
		ft_printf("\n");
	}
}

int	check_secondary_diagonal(t_board *board, int player)
{
	int	consec;
	int	i;
	int	j;
	int	k;

	for (i = 0; i < board->heigth - 3; i++)
	{
		for (j = board->width - 1; j > 2; j--)
		{
			consec = 0;
			for (k = 0; k < 4; k++)
				if (board->cells[i + k][j - k] == player)
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

int	check_primary_diagonal(t_board *board, int player)
{
	int	consec;
	int	i;
	int	j;
	int	k;

	for (i = 0; i < board->heigth - 3; i++)
	{
		for (j = 0; j < board->width - 3; j++)
		{
			consec = 0;
			for (k = 0; k < 4; k++)
				if (board->cells[i + k][j + k] == player)
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

int	check_horizontal(t_board *board, int player)
{
	int	consec;
	int	i;
	int	j;

	for (i = 0; i < board->heigth; i++)
	{
		consec = 0;
		for (j = 1; j < board->width; j++)
		{
			if (board->cells[i][j] == player && board->cells[i][j - 1] == player)
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

int	check_vertical(t_board *board, int player)
{
	int	consec;
	int	i;
	int	j;

	for (i = 0; i < board->width; i++)
	{
		consec = 0;
		for (j = 1; j < board->heigth; j++)
		{
			if (board->cells[j][i] == player && board->cells[j - 1][i] == player)
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

int	check_win_states(t_board *board, int player)
{
	if (!check_primary_diagonal(board, player)
		|| !check_secondary_diagonal(board, player)
		|| !check_vertical(board, player)
		|| !check_horizontal(board, player))
		return (WIN);
	return (CONTINUE);
}

int	is_full(t_board *board)
{
	int	i;
	int	j;

	for (i = 0; i < board->heigth; i++)
	{
		for (j = 0; j < board->width; j++)
			if (board->cells[i][j] == 0)
				return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

void	check_invalid_move(t_board *board, int cur_row, int move)
{
	char	*input;

	while (!cur_row && board->cells[cur_row][move] != 0)
	{
		ft_printf("Find another move!\n");
		input = get_next_line(STDIN_FILENO);
		ft_printf("The move is:\n");
		ft_printf("%s", input);
		move = ft_atoi(input) - 1;
		cur_row = board->heigth - 1;
		while (cur_row && board->cells[cur_row][move] != 0)
			cur_row--;
	}
	return ;
}

int	player_turn(t_board *board, int player)
{
	char	*input;
	int		cur_row;
	int		move;

	cur_row = board->heigth - 1;
	input = get_next_line(0);
	if (!input)
	{
		ft_printf("Bye!\n");
		return (PLAYER_EOF);
	}
	ft_printf("The move is:\n");
	ft_printf("%s", input);
	move = ft_atoi(input) - 1;
	while (cur_row && board->cells[cur_row][move] != 0)
		cur_row--;
	check_invalid_move(board, cur_row, move);
	if (move < board->width)
		board->cells[cur_row][move] = player;
	return (move);
}

int	gameplay(t_board *board, int no_ai)
{
	int	move;

	(void)no_ai;
	print_board(board);
	while (!is_full(board))
	{
		ft_printf("Player One's turn:\n");
		move = player_turn(board, PLAYER_TURN);
		if (move == PLAYER_EOF)
			return (EXIT_SUCCESS);
		print_board(board);
		if (!check_win_states(board, PLAYER_TURN))
			return (PLAYER_WINS);
		ft_printf("Player Two's turn:\n");
		move = player_turn(board, AI_TURN);
		if (move == PLAYER_EOF)
			return (EXIT_SUCCESS);
		print_board(board);
		if (!check_win_states(board, AI_TURN))
			return (AI_WINS);
	}
	return (EXIT_FAILURE);
}

int	init_game_board(t_board *board)
{
	int	i;

	board->cells = ft_malloc(sizeof(int *) * (size_t)(board->heigth));
	for (i = 0; i < board->heigth; ++i)
		board->cells[i] = ft_calloc((size_t)(board->width), sizeof(int));
	return (EXIT_SUCCESS);
}
