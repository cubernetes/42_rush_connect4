/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkoca <dkoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 17:30:41 by tischmid          #+#    #+#             */
/*   Updated: 2024/08/04 02:38:23 by dkoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "connect4.h"
#include "libft.h"

#include <stdlib.h>

/* TODO: free as often as possible */
int	main(int argc, char *argv[], char *envp[])
{
	int **board;
	int heigth;
	int width;
	(void)set_allocator(gc_malloc);
	(void)gc_set_context("GAME");

	if (TRUE)
	{
		init_game_board(argv, &board, &heigth, &width);
		(void)gameplay(board, heigth, width);
	}
	else
	{
		init(envp);
		nc_gameplay();
		finish();
	}
	gc_free_all();
	return (EXIT_SUCCESS);
}
