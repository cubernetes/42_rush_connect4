/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkoca <dkoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 00:56:34 by dkoca             #+#    #+#             */
/*   Updated: 2024/08/04 23:33:33 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "connect4.h"
#include "libft.h"

#include <stdlib.h>
#include <unistd.h>

void	print_board(t_board *board)
{
	int	i;
	int	j;

	for (i = 0; i < board->heigth; i++)
	{
		for (j = 0; j < board->width; j++)
			ft_printf("\033[%sm\033[30m| \033[m", board->cells[i][j] == 0 ? "44" : board->cells[i][j] == PLAYER1 ? "41" : "48;5;11");
		ft_printf("\n");
	}
}

void	print_board2(int **cells, int heigth, int width)
{
	int	i;
	int	j;

	for (i = 0; i < heigth; i++)
	{
		for (j = 0; j < width; j++)
			ft_printf("\033[%sm\033[30m| \033[m", cells[i][j] == 0 ? "44" : cells[i][j] == PLAYER1 ? "41" : "48;5;11");
		ft_printf("\n");
	}
}

int	check_secondary_diagonal(int **cells, int heigth, int width, int player, int print)
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
				if (cells[i + k][j - k] == player)
					consec++;
			if (consec == 4)
			{
				if (print)
					ft_printf("Player \033[%sm  \033[m is the WINNER!\n", player == PLAYER1 ? "41" : "48;5;11");
				return (EXIT_SUCCESS);
			}
		}
	}
	return (EXIT_FAILURE);
}

int	check_primary_diagonal(int **cells, int heigth, int width, int player, int print)
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
				if (cells[i + k][j + k] == player)
					consec++;
			if (consec == 4)
			{
				if (print)
					ft_printf("Player \033[%sm  \033[m is the WINNER!\n", player == PLAYER1 ? "41" : "48;5;11");
				return (WIN);
			}
		}
	}
	return (CONTINUE);
}

int	check_horizontal(int **cells, int heigth, int width, int player, int print)
{
	int	consec;
	int	i;
	int	j;

	for (i = 0; i < heigth; i++)
	{
		consec = 0;
		for (j = 1; j < width; j++)
		{
			if (cells[i][j] == player && cells[i][j - 1] == player)
				consec++;
			else
				consec = 0;
			if (consec == 3)
			{
				if (print)
					ft_printf("Player \033[%sm  \033[m is the WINNER!\n", player == PLAYER1 ? "41" : "48;5;11");
				return (WIN);
			}
		}
	}
	return (CONTINUE);
}

int	check_vertical(int **cells, int heigth, int width, int player, int print)
{
	int	consec;
	int	i;
	int	j;

	for (i = 0; i < width; i++)
	{
		consec = 0;
		for (j = 1; j < heigth; j++)
		{
			if (cells[j][i] == player && cells[j - 1][i] == player)
				consec++;
			else
				consec = 0;
			if (consec == 3)
			{
				if (print)
					ft_printf("Player \033[%sm  \033[m is the WINNER!\n", player == PLAYER1 ? "41" : "48;5;11");
				return (WIN);
			}
		}
	}
	return (CONTINUE);
}

int	check_win_states(int **cells, int heigth, int width, int player, int print)
{
	if (!check_primary_diagonal(cells, heigth, width, player, print)
		|| !check_secondary_diagonal(cells, heigth, width, player, print)
		|| !check_vertical(cells, heigth, width, player, print)
		|| !check_horizontal(cells, heigth, width, player, print))
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
	ft_printf("It's a draw!\n");
	return (EXIT_FAILURE);
}

int	player_turn(t_board *board, int player)
{
	char	*input;
	int		cur_row;
	int		move;
	int		status;

	while (TRUE)
	{
		input = get_next_line(STDIN_FILENO);
		if (input == NULL)
		{
			ft_printf("Bye!\n");
			return (PLAYER_EOF);
		}
		move = (int)ft_atol_status(input, &status) - 1;
		if ((status & 13) == 0 && move >= 0 && move < board->width)
		{
			cur_row = board->heigth - 1;
			while (cur_row >= 0 && board->cells[cur_row][move] != 0)
				cur_row--;
			if (cur_row >= 0 && board->cells[cur_row][move] == 0)
				break ;
		}
		if (!ft_strcmp(input, "\n"))
			ft_printf("Please input something:\n");
		else
			ft_printf("Find another move:\n");
	}
	board->cells[cur_row][move] = player;
	return (move);
}

int	init_game_board(t_board *board)
{
	int	i;

	board->cells = ft_malloc(sizeof(int *) * (size_t)(board->heigth));
	for (i = 0; i < board->heigth; ++i)
		board->cells[i] = ft_calloc((size_t)(board->width), sizeof(int));
	return (EXIT_SUCCESS);
}

int	gameplay(t_board *board, int no_ai)
{
	int	move;
	int	parity;

	parity = 0;
	print_board(board);
	while (!is_full(board))
	{
		ft_printf("Player One's turn:\n");
		move = player_turn(board, PLAYER1);
		if (move == PLAYER_EOF)
			return (EXIT_SUCCESS);
		print_board(board);
		if (!check_win_states(board->cells, board->heigth, board->width, PLAYER1, TRUE))
			return (PLAYER_WINS);
		parity = (parity + 1) & 1;
		ft_printf("Player Two's turn:\n");
		if (no_ai)
		{
			move = player_turn(board, PLAYER2);
			if (move == PLAYER_EOF)
				return (EXIT_SUCCESS);
		}
		else
			ai_move(board, parity);
		print_board(board);
		if (!check_win_states(board->cells, board->heigth, board->width, PLAYER2, TRUE))
			return (AI_WINS);
		parity = (parity + 1) & 1;
	}
	return (EXIT_FAILURE);
}
