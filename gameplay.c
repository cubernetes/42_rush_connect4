/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkoca <dkoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 00:56:34 by dkoca             #+#    #+#             */
/*   Updated: 2024/08/04 21:38:30 by dkoca            ###   ########.fr       */
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
#define VALID 0
#define INVALID 1

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

int	check_secondary_diagonal(int **cells, int heigth, int width, int player)
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
				ft_printf("Player %i is the WINNER secondary!\n", player);
				return (EXIT_SUCCESS);
			}
		}
	}
	return (EXIT_FAILURE);
}

int	check_primary_diagonal(int **cells, int heigth, int width, int player)
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
				ft_printf("Player %i is the WINNER diagonal!\n", player);
				return (WIN);
			}
		}
	}
	return (CONTINUE);
}

int	check_horizontal(int **cells, int heigth, int width, int player)
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
				ft_printf("Player %i is the WINNER horizontal!\n", player);
				return (WIN);
			}
		}
	}
	return (CONTINUE);
}

int	check_vertical(int **cells, int heigth, int width, int player)
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
				ft_printf("Player %i is the WINNER vertical!\n", player);
				return (WIN);
			}
		}
	}
	return (CONTINUE);
}

int	check_win_states(int **cells, int heigth, int width, int player)
{
	if (!check_primary_diagonal(cells, heigth, width, player)
		|| !check_secondary_diagonal(cells, heigth, width, player)
		|| !check_vertical(cells, heigth, width, player)
		|| !check_horizontal(cells, heigth, width, player))
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

int	check_invalid_move(t_board *board, char *input, int *cur_row, int *move, int width)
{
	char	*tmp = input;
		
	while(tmp && *tmp != '\n')
	{
		if (ft_isdigit(*tmp) == 0)
		{
			ft_printf("Move must be a digit!\n");
			return (INVALID);
		}
		tmp++;
	}
	*move = ft_atoi(input) - 1;
	while (*move < 0 || *move >= width)
	{
		ft_printf("The move is:\n");
		ft_printf("%s", input);
		ft_printf("Move must be within board limits!\n");
		return (INVALID);
	}
	*cur_row = board->heigth - 1;
	while (*cur_row && board->cells[*cur_row][*move] != 0)
		(*cur_row)--;
	while (!(*cur_row) && board->cells[*cur_row][*move] != 0)
	{
		ft_printf("The move is:\n");
		ft_printf("%s", input);
		ft_printf("Find another move!\n");
		return (INVALID);
	}
	return (VALID);
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
	while (check_invalid_move(board, input, &cur_row, &move, board->width))
	{
		input = get_next_line(0);
		if (!input)
		{
			ft_printf("Bye!\n");
			return (PLAYER_EOF);
		}
		move = ft_atoi(input) - 1;
	}
	if (move < board->width)
		board->cells[cur_row][move] = player;
	return (move);
}

int	rollout(t_board *board);

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
		if (!check_win_states(board->cells, board->heigth, board->width, PLAYER_TURN))
			return (PLAYER_WINS);
		ft_printf("Player Two's turn:\n");
		move = player_turn(board, AI_TURN);
		// rollout(board);
		if (move == PLAYER_EOF)
			return (EXIT_SUCCESS);
		print_board(board);
		if (!check_win_states(board->cells, board->heigth, board->width, AI_TURN))
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



// int is_terminal_state(t_board *board)
// {
// 	if (!check_win_states(board, AI_TURN)
// 		|| !check_win_states(board, PLAYER_TURN)
// 		|| is_full(board))
// 		return (1);
// 	return (0);
// }

// int empty_cols(t_board *board, int *avail_cols)
// {
// 	int cols;
// 	int rows;
// 	int empty = 0;
	
// 	for (rows = 0; rows < board->heigth; rows++)
// 	{
// 		for (cols = 0; cols < board->width; cols++)
// 		{
// 			if (board->cells[rows][cols] == 0 && avail_cols[cols] == 0)
// 				avail_cols[cols] = 1;
// 			else if (board->cells[rows][cols] != 0 && avail_cols[cols] != 1)
// 				avail_cols[cols] = 0;
// 		}
// 	}
// 	for (cols = 0; cols < board->width; cols++)
// 	{
// 		if (avail_cols[cols])
// 			empty++;
// 	}
// 	return (empty);
// }

// #include <time.h>

// int available_moves(t_board *board)
// {
// 	srand((unsigned int)time(NULL));
// 	int *avail_cols;
// 	int empty = 0;
// 	int move = -1;

// 	avail_cols = ft_calloc((size_t)board->width, sizeof(int));
// 	if ((empty = empty_cols(board, avail_cols)))
// 	{
// 		move = rand() % empty;
// 		while (avail_cols[move] == 0)
// 		{
// 			move = rand() % board->width;
// 		}
// 		return (move);
// 	}
// 	return(-1);		
// }

// int	rollout(t_board *board)
// {
// 	// if the visit value is 0, rollout
// 	int move;
// 	int row = board->heigth - 1;
// 	// while (!is_terminal_state(board))
// 	// {
// 	// randomly choose available moves
// 		if ((move = available_moves(board)) > -1)
// 			while (board->cells[row][move] != 0 && row >= 0)
// 				row--;
// 		board->cells[row][move] = AI_TURN;
// 	// simulate those actions, and reassign the current state
// 	// }
// 	return (0);
// }
