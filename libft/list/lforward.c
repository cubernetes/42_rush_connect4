/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lforward.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 00:59:32 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 00:59:37 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/** Forward-advance the iterator of a list.
 *
 *  @param list: the list to advance the iterator of
 *  @returns: the list node that the list's iterator points to after advancing;
 *            NULL after the last element and pops from the iterator stack
 */
t_list_node	*lforward(t_list list[static 1])
{
	if (list->_iterator_stack->len == 0)
		return (NULL);
	if (list->current_idx == list->len - 1)
	{
		list->current = list->_iterator_stack->last->as_literator->current;
		list->current_idx
			= list->_iterator_stack->last->as_literator->current_idx;
		list->_method = list->_iterator_stack->last->as_literator->method;
		(void)lpop(list->_iterator_stack);
		return (NULL);
	}
	list->current = list->current->next;
	list->current_idx++;
	return (list->current);
}
