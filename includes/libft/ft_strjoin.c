/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nihamila <nihamila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 14:29:42 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/21 10:53:47 by nihamila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char *str;
	size_t len1;
	size_t len2;

	if (!s1 || !s2)
		return NULL;
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	if (len1 + len2 + 1 == 0)
		return NULL;
	str = malloc(sizeof(char) * (len1 + len2 + 1));
	if (!str)
		return NULL;
	ft_strlcpy(str, s1, len1 + 1);
	ft_strlcat(str, s2, len1 + len2 + 1);
	str[len1 + len2] = '\0';
	return (str);
}

/*char	*ft_strjoin(char *s1, char *s2)
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
}*/

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
