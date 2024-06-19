/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 14:29:42 by aguezzi           #+#    #+#             */
/*   Updated: 2024/05/29 15:12:30 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char *s1, char *s2)
{
	size_t	i;
	size_t	j;
	char	*str;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (ft_strdup(""));
	str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (str == NULL)
		return (NULL);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		str[i] = s2[j];
		i++;
		j++;
	}
	str[i] = '\0';
	return (str);
}

/*
-- j'ai ma chaine input que je recupere avec le readline
-- je cree une chaine vide ""
-- je lis ma chaine input depuis l'index 0
-- while (' ') i++;
-- tant que c'est ! d'un espace ; je suis a un index ou il n'y a pas d'espace (un caractere)
-- "" = s1
-- ft_strjoin(s1, input[i]) --> s1 = "e"
-- une fois que tu es sur un espace : tu strjoin un seul espace ; while (' ') i++;


*/