/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkoca <dkoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 17:30:41 by tischmid          #+#    #+#             */
/*   Updated: 2024/08/04 03:31:10 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "connect4.h"
#include "libft.h"

#include <stdlib.h>

int	usage(char *name)
{
	ft_printf("Usage: %s [--no-ai] [--no-fancy] ROWS COLS\n"
		"ROWS must be bigger than or equal to 6\n",
		"COLS must be bigger than or equal to 7\n",
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
	*width = (int)ft_atol_status(*argv, &status);
	if (*width < 7)
		return (usage(name));
	if (status & 13)
		return (usage(name));
	++argv;
	*heigth = (int)ft_atol_status(*argv, &status);
	if (*heigth < 6)
		return (usage(name));
	if (status & 13)
		return (usage(name));
	return (no_ai | (no_fancy << 1));
}

/* TODO: free as often as possible */
int	main(int argc, char *argv[], char *envp[])
{
	int	**board;
	int	res;
	int	no_ai;
	int	no_fancy;
	int	width;
	int	heigth;

	(void)argc;
	(void)argv;
	(void)set_allocator(gc_malloc);
	(void)gc_set_context("GAME");
	if (((res = parse_args(argc, argv, &heigth, &width)) == -1))
		return (gc_free_all(), EXIT_FAILURE);
	no_ai = res & 1;
	no_fancy = res & 2;
	init_game_board(&board, heigth, width);
	if (no_fancy)
		(void)gameplay(board, heigth, width, no_ai);
	else
	{
		init(envp);
		nc_gameplay(board, heigth, width, no_ai);
		finish();
	}
	gc_free_all();
	return (EXIT_SUCCESS);
}
