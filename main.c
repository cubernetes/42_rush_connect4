/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkoca <dkoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 17:30:41 by tischmid          #+#    #+#             */
/*   Updated: 2024/08/04 01:11:04 by dkoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <stdlib.h>
#include <unistd.h>


int	main(void)
{
	// char	*line;

	set_allocator(gc_malloc);
	gc_set_context("GAME");

	gameplay();


	gc_free_all();
	return (0);
}
