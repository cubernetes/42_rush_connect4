/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkoca <dkoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 00:56:34 by dkoca             #+#    #+#             */
/*   Updated: 2024/08/04 01:48:55 by dkoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "libft.h"

#include <stdlib.h>
#include <unistd.h>

#define HEIGTH 6
#define WIDTH 7
#define PLAYER_TURN 1
#define AI_TURN 2
#define WIN 0
#define CONTINUE 1
#define PLAYER_WINS 2
#define AI_WINS 3


void print_board(int board[HEIGTH][WIDTH])
{
	int	i;
	int	j;

	ft_printf("current state of the board:\n");
	for (i = 0; i < HEIGTH; i++)
	{
		for (j = 0; j < WIDTH; j++)
			ft_printf(" %i ", board[i][j]);			
		ft_printf("\n");
	}
}

int check_secondary_diagonal(int board[HEIGTH][WIDTH], int player)
{
	int consec;
	int	i;
	int	j;
	int	k;

	for (i = 0; i < HEIGTH - 3; i++)
	{
		for (j = WIDTH - 1; j > 2; j--)
		{
			consec = 0;
			for (k = 0; k < 4; k++)
			{
				if (board[i + k][j - k] == player)
					consec++;
			}
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

int check_primary_diagonal(int board[HEIGTH][WIDTH], int player)
{
	int consec;
	int	i;
	int	j;
	int	k;

	for (i = 0; i < HEIGTH - 3; i++)
	{
		for (j = 0; j < WIDTH - 3; j++)
		{
			consec = 0;
			for (k = 0; k < 4; k++)
			{
				if (board[i + k][j + k] == player)
					consec++;
			}
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

int check_horizontal(int board[HEIGTH][WIDTH], int player)
{
	int consec;
	int	i;
	int	j;

	for (i = 0; i < HEIGTH; i++)
	{
		consec = 0;
		for (j = 1; j < WIDTH; j++)
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
int	i;

int check_vertical(int board[HEIGTH][WIDTH], int player)
{
	int consec;
	int	j;

	for (i = 0; i < WIDTH; i++)
	{
		consec = 0;
		for (j = 1; j < HEIGTH; j++)
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

int check_win_states(int board[HEIGTH][WIDTH], int player)
{
	if (!check_primary_diagonal(board, player) ||
		!check_secondary_diagonal(board, player) ||
		!check_vertical(board, player) ||
		!check_horizontal(board, player))
		return (WIN);
	return (CONTINUE);
}

int is_full(int board[HEIGTH][WIDTH])
{
	int	i;
	int	j;

	for (i = 0; i < HEIGTH; i++)
	{
		for (j = 0; j < WIDTH; j++)
		{
			if (board[i][j] == 0)
				return (EXIT_SUCCESS);			
		}
	}
	return (EXIT_FAILURE);
}

void check_invalid_move(int board[HEIGTH][WIDTH], int cur_row, int move)
{
	char *input;
	while (!cur_row && board[cur_row][move] != 0)
	{
		ft_printf("Find another move!\n");
		input = get_next_line(STDIN_FILENO);
		ft_printf("The move was\n");
		ft_printf("%s", input);
		move = ft_atoi(input) - 1;
		cur_row = HEIGTH - 1;
		while (cur_row && board[cur_row][move] != 0)
			cur_row--;
	}
	return ;
}

int player_turn(int board[HEIGTH][WIDTH], int player)
{
	char *input;
	int cur_row = HEIGTH - 1;
	int move;
	
	input = get_next_line(0);
	if (!input)
	{
		ft_printf("Bye!\n");
		return (EXIT_SUCCESS);
	}
	ft_printf("The move is:\n");
	ft_printf("%s", input);
	// apply move
	move = ft_atoi(input) - 1;
	while (cur_row && board[cur_row][move] != 0)
		cur_row--;
	check_invalid_move(board, cur_row, move);
	if (move < WIDTH)
		board[cur_row][move] = player;
	return (move);	
}


int gameplay(void)
{
	int board[HEIGTH][WIDTH] = {0};
	// int board[HEIGTH][WIDTH] = {{0, 0, 0, 0, 0, 0, 0},
	// 							{0, 0, 0, 0, 0, 0, 0},
	// 							{0, 0, 0, 0, 0, 0, 0},
	// 							{0, 0, 0, 0, 0, 0, 0},
	// 							{0, 0, 0, 0, 0, 0, 0},
	// 							{0, 0, 0, 0, 0, 0, 0}};
	check_win_states(board, PLAYER_TURN);
	int move;

	print_board(board);
	while (!is_full(board))
	{
		// get player move
		ft_printf("Player One's turn:\n");
		move = player_turn(board, PLAYER_TURN);
		if (move == EXIT_SUCCESS)
			return (EXIT_SUCCESS);
		print_board(board);
		if (!check_win_states(board, PLAYER_TURN))
			return (PLAYER_WINS);
		ft_printf("Player Two's turn:\n");
		// get ai turn (or player 2)
		move = player_turn(board, AI_TURN);
		if (move == EXIT_SUCCESS)
			return (EXIT_SUCCESS);
		print_board(board);	
		if (!check_win_states(board, AI_TURN))
			return (AI_WINS);
	}
	return (EXIT_FAILURE);
}

