/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ai.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 14:02:46 by tischmid          #+#    #+#             */
/*   Updated: 2024/08/04 16:53:45 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "connect4.h"
#include "libft.h"

typedef struct s_state
{
	char	**board;
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
void	iterate(t_node *root)
{
	t_node	*node;
	int		res;

	node = select(root);
	node = expand(node);
	res = rollout(node);
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

	(void)board;
	root = ft_malloc(sizeof(*root));
	root->state.visits = 0;
	root->parent = NULL;
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
