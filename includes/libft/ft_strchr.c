/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 13:38:58 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/25 19:56:14 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	if (!s)
		return (NULL);
	while (*s != 0)
	{
		if (*s == c % 256)
		{
			break ;
		}
		s++;
	}
	if (*s != 0)
	{
		return ((char *)s);
	}
	if (c % 256 == 0)
		return ((char *)s);
	return (NULL);
}
