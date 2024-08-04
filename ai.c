/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ai.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkoca <dkoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 14:02:46 by tischmid          #+#    #+#             */
/*   Updated: 2024/08/04 22:38:41 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "connect4.h"
#include "libft.h"
#include <float.h>

/* https://gist.github.com/LingDong-/7e4c4cae5cbbc44400a05fba65f06f23 */
float	ln(float x)
{
	unsigned int	bx;
	unsigned int	ex;
	int				t;

	bx = *(unsigned int *)(&x);
	ex = bx >> 23;
	t = (int)ex - (int)127;
	bx = 1065353216 | (bx & 8388607);
	x = *(float *)(&bx);
	return (-1.49278f + (2.11263f + (-0.729104f + 0.10969f
				* x) * x) * x + 0.6931471806f * (float)t);
}

/* https://gist.github.com/lh3/9430fbb713f8a6171ab0befecfee015d */
float	mg_sqrtf(float x)
{
	union
	{
		float		f;
		uint32_t	i;
	} u_z = {x};
	u_z.i = 0x5f3759df - (u_z.i >> 1);
	u_z.f *= (1.5f - (x * 0.5f * u_z.f * u_z.f));
	u_z.i = 0x7EEEEEEE - u_z.i;
	return (u_z.f);
}

/* TODO: turn into macro */
float	get_ucb1(t_node *node)
{
	float	c;

	c = 2;
	if (node->state.ai_turn)
		return ((float)node->state.total_score / (float)node->state.visits + mg_sqrtf(c * ln((float)node->parent->state.visits) / (float)node->state.visits));
	return (1 - (float)node->state.total_score / (float)node->state.visits + mg_sqrtf(c * ln((float)node->parent->state.visits) / (float)node->state.visits));
}

t_node	*new_node(t_node *parent, int column)
{
	t_node	*node;
	int		i;
	int		j;

	node = ft_malloc(sizeof(*node));
	node->parent = parent;
	node->children = lnew();
	node->state.visits = 0;
	node->state.total_score = 0;
	node->state.column = column;
	if (parent != NULL && column >= 0)
	{
		node->state.board = ft_malloc(sizeof(int *) * (size_t)parent->state.heigth);
		node->state.heigth = parent->state.heigth;
		node->state.width = parent->state.width;
		for (i = 0; i < parent->state.heigth; ++i)
		{
			node->state.board[i] = ft_malloc(sizeof(int) * (size_t)parent->state.width);
			for (j = 0; j < parent->state.width; ++j)
				node->state.board[i][j] = parent->state.board[i][j];
		}
		node->state.ai_turn = !parent->state.ai_turn;
		make_move(node->state.board, node->state.heigth, node->state.width, column, node->state.ai_turn ? PLAYER1 : PLAYER2); // TODO: logic correct?
	}
	else
		node->state.ai_turn = FALSE;
	return (node);
}

void	ft_putstr_n_clr(const char *str, int n, bool color)
{
	int	i;

	i = -1;
	while (++i < n)
	{
		if (color)
		{
			ft_putstr("\033[");
			ft_putnbr((i % 7) + 31);
			ft_putstr("m");
		}
		ft_printf("%s", str);
		if (color)
			ft_putstr("\033[m");
	}
}

void	print_tree_with_depth(t_node *root, int n)
{
	ft_putstr_n_clr("|   ", n, true);
	ft_printf("- %d/%d, (children: %d)\n",
			root->state.total_score,
			root->state.visits,
			root->children->len);
	if (root->children->len > 0)
	{
		liter(root->children);
		while (lnext(root->children))
			print_tree_with_depth((t_node *)root->children->current->as_ptr, n + 1);
	}
}

void	print_tree(t_node *root)
{
	print_tree_with_depth(root, 0);
}

t_node	*select(t_node *node)
{
	t_node	*best_node;
	t_node	*candidate_node;
	float	highest_ucb1;
	float	ucb1;

	if (node == NULL)
		return (NULL);
	if (node->children == NULL)
		return (node);
	highest_ucb1 = FLT_MIN;
	liter(node->children);
	best_node = NULL;
	while (lnext(node->children))
	{
		candidate_node = (t_node *)node->children->current->as_ptr;
		ucb1 = get_ucb1(candidate_node);
		if (!(ucb1 > FLT_MIN)) // Infinity
			return (candidate_node);
		if (ucb1 > highest_ucb1)
		{
			highest_ucb1 = ucb1;
			best_node = candidate_node;
		}
	}
	return (best_node);
}

t_node	*select_recursively(t_node *node, int max_children)
{
	if (node == NULL)
		return (NULL);
	while (1)
	{
		if (node->children == NULL)
			return (node);
		else if (node->children->len < (size_t)max_children)
			return (node);
		node = select(node);
	}
}

t_node	*expand(t_node *node)
{
	return ((t_node *)lpush(node->children, as_ptr(new_node(node, (int)node->children->len)))->as_ptr);
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

int is_terminal_state(t_node *node, int ai_turn)
{
	if (!check_win_states(node->state.board, node->state.heigth, node->state.width, PLAYER2, FALSE))
		return (ai_turn ? 1 : -1);
	if (!check_win_states(node->state.board, node->state.heigth, node->state.width, PLAYER1, FALSE))
		return (ai_turn ? -1 : 1);
	if (is_draw(node))
		return (0);
	return (-2);
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
	return (-1);		
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
	int	score;
	
	avail_cols = ft_calloc((size_t)node->state.width, sizeof(int));
	new_board = ft_malloc(sizeof(int *) * (size_t)(node->state.heigth));
	for (i = 0; i < node->state.heigth; ++i)
		new_board[i] = ft_calloc((size_t)(node->state.width), sizeof(int));
	copy_board(node->state.board, new_board, node->state.heigth, node->state.width);
	// loop while not terminal state
	while ((score = is_terminal_state(node, node->state.ai_turn)) == -2)
	{
	// randomly choose available moves
		if ((move = available_moves(node, avail_cols)) > -1)
			while (row >= 0 && node->state.board[row][move] != 0)
				row--;
		else
			break ;
		if (row >= 0 && node->state.board[row][move] == 0)
			node->state.board[row][move] = PLAYER2;
		else
		{
				int j;

				for (i = 0; i < node->state.heigth; i++)
				{
					for (j = 0; j < node->state.width; j++)
						ft_printf("\033[%sm\033[30m| \033[m", node->state.board[i][j] == 0 ? "44" : node->state.board[i][j] == PLAYER1 ? "41" : "48;5;11");
					ft_printf("\n");
				}
			if (row >= 0)
				ft_printf("ERROR: %d, row:%d, cell:%d\n", move, row, node->state.board[row][move]);
			else
				ft_printf("ERROR: %d, row:%d\n", move, row);
		}
	// simulate those actions, and reassign the current state
	}
	return (score);
}

void	backprop(t_node *node, int res)
{
	(void)node;
	(void)res;
}

/* classic 4-step monte carlo search on trees */
void	iterate(t_node *root, int max_children)
{
	t_node	*node;
	int		res = 0;

	node = select_recursively(root, max_children);
	node = expand(node);
	/* res = rollout(node); */
	/* ft_printf("Result of the simulation:%d\n", res); */
	/* print_tree(root); */
	/* ft_printf("\n"); */
	backprop(node, res);
}

/* return the column with the best UCT or visit value */
/* using a monte carlo search tree */
int	mcts(t_board *board)
{
	t_node	*root;
	int		i;
	int		column;

	root = new_node(NULL, -1);
	root->state.board = board->cells;
	root->state.heigth = board->heigth;
	root->state.width = board->width;
	i = -1;
	while (++i < MAX_ITER)
		iterate(root, board->width);
	column = select(root)->state.column;
	return (column);
}

void	ai_move(t_board *board, int parity)
{
	int	column;

	column = mcts(board);
	make_move(board->cells, board->heigth, board->width, column,
		parity ? PLAYER2 : PLAYER1);
}
