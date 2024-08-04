/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ai.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 14:02:46 by tischmid          #+#    #+#             */
/*   Updated: 2024/08/04 20:58:08 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "connect4.h"
#include "libft.h"
#include <float.h>

# define MAX(a, b) ((a) > (b) ? (a) : (b))

/* TODO: maybe change back to char**? */
typedef struct s_state
{
	int		**board;
	int		heigth;
	int		width;
	int		total_score;
	int		visits;
	int		ai_turn;
	int		column;
}	t_state;

typedef struct s_node
{
	struct s_node	*parent;
	t_list			*children;
	t_state			state;
}	t_node;

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

int	rollout(t_node *node)
{
	(void)node;
	return (0);
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
	int		res;

	node = select_recursively(root, max_children);
	node = expand(node);
	res = rollout(node);
	backprop(node, res);
}

/* return the column with the best UCT or visit value */
/* using a monte carlo search tree */
int	mcts(t_board *board)
{
	t_node	*root;
	int		i;
	int		column;

	(void)board;
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
		parity ? PLAYER1 : PLAYER2);
}
