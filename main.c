/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkoca <dkoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 17:30:41 by tischmid          #+#    #+#             */
/*   Updated: 2024/08/04 18:55:56 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "connect4.h"
#include "libft.h"

#include <stdlib.h>

int	usage(char *name)
{
	ft_printf("Usage: %s [--no-ai] [--no-fancy] ROWS COLS\n"
		"\tROWS must at least 6\n"
		"\tCOLS must at least 7\n",
		name);
	return (-1);
}

int	parse_args(int argc, char *argv[], int *heigth, int *width)
{
	int		no_ai;
	int		no_fancy;
	int		status;
	char	*name;

	name = argv[0];
	if (argc < 3)
		return (usage(name));
	++argv;
	--argc;
	no_ai = FALSE;
	no_fancy = FALSE;
	while (!ft_strcmp("--no-ai", *argv) || !ft_strcmp("--no-fancy", *argv))
	{
		if (!ft_strcmp("--no-ai", *argv))
			no_ai = TRUE;
		else
			no_fancy = TRUE;
		++argv;
		--argc;
	}
	if (argc != 2)
		return (usage(name));
	*heigth = (int)ft_atol_status(*argv, &status);
	if (*heigth < 6)
		return (usage(name));
	if (status & 13)
		return (usage(name));
	++argv;
	*width = (int)ft_atol_status(*argv, &status);
	if (*width < 7)
		return (usage(name));
	if (status & 13)
		return (usage(name));
	return (no_ai | (no_fancy << 1));
}

/* TODO: free as often as possible */
int	main(int argc, char *argv[], char *envp[])
{
	t_board	board;
	int		res;
	int		no_ai;
	int		no_fancy;
	int		width;
	int		heigth;

	(void)argc;
	(void)argv; // TODO: Remove (void) stuff
	(void)envp;
	(void)set_allocator(gc_malloc);
	(void)gc_set_context("GAME");
	if (((res = parse_args(argc, argv, &heigth, &width)) == -1))
		return (gc_free_all(), EXIT_FAILURE);
	no_ai = res & 1;
	no_fancy = res & 2;
	board.width = width;
	board.heigth = heigth;
	init_game_board(&board);
	if (no_fancy)
		(void)gameplay(&board, no_ai);
	else
	{
		/* init(envp); */
		nc_gameplay(&board, no_ai);
		/* finish(); */
	}
	gc_free_all();
	return (EXIT_SUCCESS);
}
