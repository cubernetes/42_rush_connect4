/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 17:30:41 by tischmid          #+#    #+#             */
/*   Updated: 2024/08/03 17:32:56 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <stdlib.h>

int	main(void)
{
	char	*line;

	set_allocator(gc_malloc);
	gc_set_context("DEFAULT");
	ft_printf("Hi\n");
	line = get_next_line(0);
	ft_printf("The line was\n");
	ft_printf("%s", line);
	gc_free_all();
	return (0);
}
