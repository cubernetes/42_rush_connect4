/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkoca <dkoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 17:30:41 by tischmid          #+#    #+#             */
/*   Updated: 2024/08/04 02:53:37 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "connect4.h"
#include "libft.h"

#include <stdlib.h>

int	parse_args(int argc, char *argv[], int *width, int *height)
{
	int		no_ai;
	int		no_fancy;
	int		status;
	char	*name;

	name = argv[0];
	if (argc < 3)
	{
		ft_printf("Usage: %s [--no-ai] [--no-fancy] ROWS COLS\n",
			name);
		return (-1);
	}
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
	{
		ft_printf("Usage: %s [--no-ai] [--no-fancy] ROWS COLS\n",
			name);
		return (-1);
	}
	*width = (int)ft_atol_status(*argv, &status);
	if (status & 13)
	{
		ft_printf("Usage: %s [--no-ai] [--no-fancy] ROWS COLS\n",
			name);
		return (-1);
	}
	*height = (int)ft_atol_status(*argv, &status);
	if (status & 13)
	{
		ft_printf("Usage: %s [--no-ai] [--no-fancy] ROWS COLS\n",
			name);
		return (-1);
	}
	return (no_ai | (no_fancy << 1));
}

/* TODO: free as often as possible */
int	main(int argc, char *argv[], char *envp[])
{
	int	res;
	int	no_ai;
	int	no_fancy;
	int	width;
	int	height;

	if (((res = parse_args(argc, argv, &width, &height)) == -1))
		return (EXIT_FAILURE);
	no_ai = res & 1;
	no_fancy = res & 2;
	(void)argc;
	(void)argv;
	(void)set_allocator(gc_malloc);
	(void)gc_set_context("GAME");
	if (no_fancy)
		(void)gameplay(no_ai);
	else
	{
		init(envp);
		nc_gameplay(no_ai);
		finish();
	}
	gc_free_all();
	return (EXIT_SUCCESS);
}
