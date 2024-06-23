/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signaux.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nihamila <nihamila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 16:01:26 by nihamila          #+#    #+#             */
/*   Updated: 2024/06/21 16:04:12 by nihamila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_ctrl_c(int sig)
{
	(void) sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	ft_ctrl_sl(int i)
{
	(void) i;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	ft_ctrl_d(void)
{
	return ;
}

void	handle_prompt(void)
{
	signal(SIGQUIT, ft_ctrl_sl);
	signal(SIGINT, ft_ctrl_c);
}
