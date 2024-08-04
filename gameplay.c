/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkoca <dkoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 00:56:34 by dkoca             #+#    #+#             */
/*   Updated: 2024/08/04 21:38:55 by tischmid         ###   ########.fr       */
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
			ft_printf("\033[%sm\033[30m| \033[m", board->cells[i][j] == 0 ? "44" : board->cells[i][j] == PLAYER_TURN ? "41" : "48;5;11");
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
				ft_printf("Player \033[%sm  \033[m is the WINNER!\n", player == PLAYER_TURN ? "41" : "48;5;11");
				return (EXIT_SUCCESS);
			}
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
				ft_printf("Player \033[%sm  \033[m is the WINNER!\n", player == PLAYER_TURN ? "41" : "48;5;11");
				return (WIN);
			}
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
				ft_printf("Player \033[%sm  \033[m is the WINNER!\n", player == PLAYER_TURN ? "41" : "48;5;11");
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
				ft_printf("Player \033[%sm  \033[m is the WINNER!\n", player == PLAYER_TURN ? "41" : "48;5;11");
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
		move = player_turn(board, PLAYER_TURN);
		if (move == PLAYER_EOF)
			return (EXIT_SUCCESS);
		print_board(board);
		if (!check_win_states(board, PLAYER_TURN))
			return (PLAYER_WINS);
		parity = (parity + 1) & 1;
		ft_printf("Player Two's turn:\n");
		if (no_ai)
		{
			move = player_turn(board, AI_TURN);
			if (move == PLAYER_EOF)
				return (EXIT_SUCCESS);
		}
		else
			ai_move(board, parity);
		print_board(board);
		if (!check_win_states(board, AI_TURN))
			return (AI_WINS);
		parity = (parity + 1) & 1;
	}
	return (EXIT_FAILURE);
}
