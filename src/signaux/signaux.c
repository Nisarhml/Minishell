/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signaux.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nihamila <nihamila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 16:01:26 by nihamila          #+#    #+#             */
/*   Updated: 2024/06/25 10:29:24 by nihamila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	basic_signal(int signal)
{
	if (signal == SIGINT)
	{
		set_exit_status(128 + 2);
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	if (signal == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();
	}
}

void	command_signal(int signal)
{
	if (signal == SIGINT)
	{
		set_exit_status(128 + 2);
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
	if (signal == SIGQUIT)
	{
		set_exit_status(128 + 3);
		ft_putstr_fd("Quit 3\n", STDOUT_FILENO);
	}
	exit(*get_exit_status());
}

void	set_basic_signals(void)
{
	signal(SIGINT, basic_signal);
	signal(SIGQUIT, basic_signal);
}

void	set_command_signals(void)
{
	signal(SIGINT, command_signal);
	signal(SIGQUIT, command_signal);
}

void	handle_prompt(void)
{
	set_basic_signals();
}
