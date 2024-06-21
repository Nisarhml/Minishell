/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollars_why.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nihamila <nihamila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 16:57:24 by nihamila          #+#    #+#             */
/*   Updated: 2024/06/21 18:23:17 by nihamila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_last_status(char *input)
{
	char	*expanded = NULL;
	char	*status_str;
	int		g_last_status;

	expanded = NULL;
	status_str = ft_itoa(g_last_status); // Convertir g_last_status en chaîne de caractères
	if (status_str == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE); // Gestion de l'erreur de mémoire
	}
	expanded = build_expanded(input, status_str); // Construire la chaîne de caractères étendue
	free(status_str); // Libérer la mémoire allouée pour status_str
	return (expanded); // Retourner la chaîne de caractères étendue
}
char	*build_expanded(char *input, char *status_str)
{
	size_t	input_len;
	size_t	status_len;
	char	*expanded;
	char	*pos;
	char	*current;

	input_len = ft_strlen(input);
	status_len = ft_strlen(status_str);
	expanded = malloc(input_len + status_len + 1); // +1 pour le '\0' de fin de chaîne
	if (expanded == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE); // Gestion de l'erreur de mémoire
	}
	pos = input;
	current = expanded;
	while (*pos != '\0')
	{
		if (ft_strncmp(pos, "$?", 2) == 0)
		{
			mini_strcpy(current, status_str);
			current += status_len;
			pos += 2;
		}
		else
			*current++ = *pos++;
	}
	*current = '\0';
	return expanded;
}

void	replace_and_copy(char *dest, char *src, size_t n)
{
	while (*src && n)
	{
		*dest++ = *src++;
		n--;
	}
	*dest = '\0';
}

char	*find_position(char *input, char *status_str)
{
	char	*pos;
	char	*expanded;

	pos = mini_strstr(input, "$?");
	if (pos != NULL)
	{
		expanded = malloc(ft_strlen(input) + ft_strlen(status_str) - 2 + 1); // -2 pour "$?", +1 pour '\0'
		if (expanded == NULL)
		{
			perror("malloc");
			exit(EXIT_FAILURE); // Gestion de l'erreur de mémoire
		}
		replace_and_copy(expanded, input, pos - input);
		mini_strcat(expanded, status_str);
		mini_strcat(expanded, pos + 2);
		return (expanded);
	}
	return (ft_strdup(input));
}

void	update_last_status(int status)
{
	g_last_status = status;
}
