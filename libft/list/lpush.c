/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lpush.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 00:58:27 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 00:58:27 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libft.h"

/** Push a data union to the end of a list.
 *
 *  @param list: the list to push `data' to
 *  @param data: the data union to push to `list'
 *  @returns: the node added
 */
t_list_node	*lpush(t_list list[static 1], t_data data)
{
	struct s_list_node	*node;

	node = ft_malloc(sizeof(*node));
	ft_memmove(node, &data, sizeof(data));
	if (list->len == 0)
	{
		list->first = node;
		list->current = NULL;
		list->last = node;
	}
	else
	{
		list->last->next = node;
		list->first->prev = node;
	}
	node->next = list->first;
	node->prev = list->last;
	list->last = node;
	list->len++;
	return (list->last);
}
