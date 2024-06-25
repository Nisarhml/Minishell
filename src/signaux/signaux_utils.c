/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signaux_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nihamila <nihamila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 10:25:02 by nihamila          #+#    #+#             */
/*   Updated: 2024/06/25 10:32:34 by nihamila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	*get_exit_status(void)
{
	static int	exit_status;

	return (&exit_status);
}

void	set_exit_status(int status)
{
	*get_exit_status() = status;
}

void	set_here_doc_signals(void)
{
	signal(SIGINT, here_doc_signal);
}

void	here_doc_signal(int signal)
{
	if (signal == SIGINT)
	{
		set_exit_status(1);
		printf("\n");
		exit(*get_exit_status());
	}
}

void	ignore_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
