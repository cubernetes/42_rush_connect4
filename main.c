/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkoca <dkoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 17:30:41 by tischmid          #+#    #+#             */
/*   Updated: 2024/08/04 02:14:23 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "connect4.h"
#include "libft.h"

#include <stdlib.h>

/* TODO: free as often as possible */
int	main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	(void)set_allocator(gc_malloc);
	(void)gc_set_context("GAME");
	if (TRUE)
		(void)gameplay();
	else
	{
		init(envp);
		nc_gameplay();
		finish();
	}
	gc_free_all();
	return (EXIT_SUCCESS);
}
