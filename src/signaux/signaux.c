/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signaux.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 16:01:26 by nihamila          #+#    #+#             */
/*   Updated: 2024/06/24 15:02:03 by aguezzi          ###   ########.fr       */
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

void	set_here_doc_signals(void)
{
	signal(SIGINT, here_doc_signal);
}

/*void	ft_ctrl_c(int sig)
{
	(void) sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	ft_ctrl_sl(int i) // cette fonction ne sert a rien car le Ctrl+\ ne doit rien faire
{
	(void) i;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	ft_ctrl_d(void)
{
	return ;
}*/

void	handle_prompt(void)
{
	set_basic_signals();
}
