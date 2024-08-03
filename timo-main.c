/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timo-main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 17:30:41 by tischmid          #+#    #+#             */
/*   Updated: 2024/08/03 18:53:56 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* TODO: make sure <curses.h> is NOT included */
#include <locale.h> /* forbidden header */
#include <stdlib.h>
#include <ncurses.h>

void	ft_setenv(char *envp[], const char *name, const char *value)
{
	char	**parts;
	char	*equal_value;

	equal_value = ft_strjoin("=", value);
	while (*envp)
	{
		parts = ft_split(*envp, '=');
		if (!ft_strcmp(parts[0], name))
		{
			*envp = ft_strjoin(name, equal_value);
			break ;
		}
		++envp;
	}
}

void	init(char *envp[])
{
	set_allocator(gc_malloc);
	gc_set_context("GAME");
	ft_setenv(envp, "TERM", "xterm-256color");
	/* setlocale(LC_ALL, ""); */ /* forbidden function */
	initscr();
	keypad(stdscr, TRUE);
	noecho();
	cbreak();
	curs_set(0);
	start_color();
	set_escdelay(0);
}

void	finish(void)
{
	gc_free_all();
	endwin();
}

int	main(int argc, char *argv[], char *envp[])
{
	ft_printf("BEFORE INIT\n");
	init(envp);
	ft_printf("GAME\n");
	finish();
	ft_printf("AFTER FINISH\n");
	return (EXIT_SUCCESS);
}
