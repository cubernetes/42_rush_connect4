/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ai.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkoca <dkoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 14:02:46 by tischmid          #+#    #+#             */
/*   Updated: 2024/08/04 21:39:55 by dkoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "connect4.h"
#include "libft.h"

typedef struct s_state
{
	int	**board;
	int heigth;
	int width;
	double	uct;
	int		total_score;
	int		visits;
}	t_state;

typedef struct s_node
{
	struct s_node	*parent;
	t_list			*children;
	t_state			state;
}	t_node;

/* https://gist.github.com/LingDong-/7e4c4cae5cbbc44400a05fba65f06f23 */
inline double	ln(float x)
{
	unsigned int	bx;
	unsigned int	ex;
	int				t;

	bx = *(unsigned int *)(&x);
	ex = bx >> 23;
	t = (int)ex - (int)127;
	bx = 1065353216 | (bx & 8388607);
	x = *(float *)(&bx);
	return (-1.49278 + (2.11263 + (-0.729104 + 0.10969
				* x) * x) * x + 0.6931471806 * t);
}

/* https://gist.github.com/lh3/9430fbb713f8a6171ab0befecfee015d */
inline float mg_sqrtf(float x)
{
	union { float f; uint32_t i; } z = { x };
	z.i  = 0x5f3759df - (z.i >> 1);
	z.f *= (1.5f - (x * 0.5f * z.f * z.f));
	z.i = 0x7EEEEEEE - z.i;
	return z.f;
}

t_node	*select(t_node *root)
{
	return (root);
}

t_node	*expand(t_node *node)
{
	return (node);
}


int	is_draw(t_node *node)
{
	int	i;
	int	j;

	for (i = 0; i < node->state.heigth; i++)
	{
		for (j = 0; j < node->state.width; j++)
			if (node->state.board[i][j] == 0)
				return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

int is_terminal_state(t_node *node)
{
	if (!check_win_states(node->state.board, node->state.heigth, node->state.width, AI_TURN)
		|| !check_win_states(node->state.board, node->state.heigth, node->state.width, PLAYER_TURN)
		|| is_draw(node))
		return (1);
	return (0);
}

int empty_cols(t_node *node, int *avail_cols)
{
	int cols;
	int rows;
	int empty = 0;
	
	for (rows = 0; rows < node->state.heigth; rows++)
	{
		for (cols = 0; cols < node->state.width; cols++)
		{
			if (node->state.board[rows][cols] == 0 && avail_cols[cols] == 0)
				avail_cols[cols] = 1;
			else if (node->state.board[rows][cols] != 0 && avail_cols[cols] != 1)
				avail_cols[cols] = 0;
		}
	}
	for (cols = 0; cols < node->state.width; cols++)
	{
		if (avail_cols[cols])
			empty++;
	}
	return (empty);
}

int available_moves(t_node *node, int *avail_cols)
{
	srand((unsigned int)time(NULL));
	// int *avail_cols;
	int empty = 0;
	int move = -1;

	// avail_cols = ft_calloc((size_t)node->state.width, sizeof(int));
	if ((empty = empty_cols(node, avail_cols)))
	{
		move = rand() % empty;
		while (avail_cols[move] == 0)
			move = rand() % node->state.width;
		return (move);
	}
	return(-1);		
}

void	copy_board(int **board, int **copied_board, int heigth, int width)
{
	int x;
	int y;
	
	for (y = 0; y < heigth; y++)
	{
		for (x = 0; x < width; x++)
		{
			copied_board[y][x] = board[y][x];
		}
	}
}

int	rollout(t_node *node)
{
	// if the visit value is 0, rollout
	int move;
	int row = node->state.heigth - 1;
	int *avail_cols;
	int **new_board;
	int	i;
	
	avail_cols = ft_calloc((size_t)node->state.width, sizeof(int));

	new_board = ft_malloc(sizeof(int *) * (size_t)(node->state.heigth));
	for (i = 0; i < node->state.heigth; ++i)
		new_board[i] = ft_calloc((size_t)(node->state.width), sizeof(int));
	copy_board(node->state.board, new_board, node->state.heigth, node->state.width);
	// loop while not terminal state
	while (!is_terminal_state(node))
	{
	// randomly choose available moves
		if ((move = available_moves(node, avail_cols)) > -1)
			while (node->state.board[row][move] != 0 && row >= 0)
				row--;
		else
			break;
		node->state.board[row][move] = AI_TURN;
	// simulate those actions, and reassign the current state
	}
	return (0);
}

void	backprop(t_node *node, int res)
{
	(void)node;
	(void)res;
}

/* classic 4-step monte carlo search on trees */
void	iterate(t_node *root)
{
	t_node	*node;
	int		res = 0;

	node = select(root);
	node = expand(node);
	// res = rollout(node);
	backprop(node, res);
}

int	choose_best(t_node *root)
{
	(void)root;
	return (0);
}

/* return the column with the best UCT or visit value */
/* using a monte carlo search tree */
int	mcts(t_board *board)
{
	t_node	*root;
	int		i;
	int		column;

	// (void)board;
	root = ft_malloc(sizeof(*root));
	root->state.visits = 0;
	root->parent = NULL;
	root->state.board = board->cells;
	root->state.heigth = board->heigth;
	root->state.width = board->width;
	i = -1;
	while (++i < MAX_ITER)
		iterate(root);
	column = choose_best(root);
	return (column);
}

void	ai_move(t_board *board)
{
	int	column;

	column = mcts(board);
	make_move(board, column, AI_MOVE);
}
