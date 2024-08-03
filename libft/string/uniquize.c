/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uniquize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 01:02:58 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 01:03:01 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/** Return a new string that is fenced with two constant unique strings,
 *  such that the whole string is unique.
 *
 *  @param str: the string to be fenced
 *  @returns: a new string the is fenced with two constant unique strings
 */
char	*uniquize(const char str[static 1])
{
	return (ft_strjoin(UNIQUE1, ft_strjoin(str, UNIQUE2)));
}
