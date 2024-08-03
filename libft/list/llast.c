/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   llast.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 00:58:48 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 00:58:48 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/** End the iterator prematurely.
 *
 *  @param list: the list to end the iteration of
 *  @returns: `list'
 */
t_list	*llast(t_list list[static 1])
{
	list->current = list->_iterator_stack->last->as_literator->current;
	list->current_idx = list->_iterator_stack->last->as_literator->current_idx;
	list->_method = list->_iterator_stack->last->as_literator->method;
	(void)lpop(list->_iterator_stack);
	return (list);
}
/* TODO: Edge case? calling llast when there was never any iterator */
