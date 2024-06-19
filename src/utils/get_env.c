/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 10:32:19 by nihamila          #+#    #+#             */
/*   Updated: 2024/06/18 17:57:37 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_env(char **env)
{
	int	i;

	i = 0;
	while(env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
}

char	*get_path(char **env)
{
	int		i;
	char	*path_prefix;
	size_t	path_len;

	i = 0;
	path_prefix = "PATH=";
	path_len = ft_strlen(path_prefix);
	while(env[i])
	{
		if (ft_strncmp(env[i], path_prefix, path_len))
			return (ft_strdup(env[i] + path_len));
		i++;
	}
	return (0);
}
