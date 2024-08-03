/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   liter_rev.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 00:59:17 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 00:59:25 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libft.h"

/** Start a new backwards-iterator on a list.
 *
 *  @param list: the list to start the backwards-iterator on
 *  @returns: `list'
 */
t_list	*liter_rev(t_list list[static 1])
{
	(void)lpush(list->_iterator_stack, as_literator(ft_memdup(&(t_literator){\
			.current = list->current, \
			.current_idx = list->current_idx, \
			.method = list->_method \
		}, \
		sizeof(t_literator) \
	)));
	if (list->last)
		list->current = list->last->next;
	else
		list->current = NULL;
	list->current_idx = (size_t)(-1);
	list->_method = lbackward;
	return (list);
}
