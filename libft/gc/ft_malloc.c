/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 01:04:13 by tischmid          #+#    #+#             */
/*   Updated: 2024/08/03 18:53:30 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>

void	*ft_malloc(size_t size)
{
	void	*ptr;

	ptr = (get_allocator())(size);
	if (!ptr)
	{
		/* TODO: strerror is also not allowed */
		/* ft_printf("\033[41;30mft_malloc: %s\033[m\n\033[31mCallstack " */
			/* "(reverse):\033[m\n", strerror(errno)); */
		print_callstack();
		/* TODO: Hehe, exit is not allowed *sad noise* */
		/* exit(EXIT_FAILURE); */
	}
	return (ptr);
}
