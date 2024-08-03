/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 01:02:09 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 01:02:10 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

/** Return the next available line including the newline character (if
 *  available) from a file descriptor.
 *
 *  @param fd: the file descriptor to read from
 *  @returns: a single line, potentially including a newline character (0x0a)
 */
char	*get_next_line(int fd)
{
	ssize_t	bytes_read;
	char	c;
	t_list	*chars;
	char	*str;

	bytes_read = read(fd, &c, 1);
	if (bytes_read < 0)
		return (NULL);
	chars = lnew();
	while (bytes_read)
	{
		lpush(chars, as_char(c));
		if (c == '\n')
			break ;
		bytes_read = read(fd, &c, 1);
	}
	if (bytes_read == 0 && chars->len == 0)
		return (NULL);
	str = ft_malloc(sizeof(*str) * (chars->len + 1));
	str[chars->len] = 0;
	liter(chars);
	while (lnext(chars))
		str[chars->current_idx] = chars->current->as_char;
	return (str);
}
/* this implementation reads character by character */
/* TODO: Test rigorously */
