/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pakopian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 18:15:07 by pakopian          #+#    #+#             */
/*   Updated: 2025/06/07 18:15:09 by pakopian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static volatile int	g_acknowledged = 0;

static void	handle_ack(int sig)
{
	(void)sig;
	g_acknowledged = 1;
}

static void	send_char(pid_t pid, unsigned char c)
{
	int	bit;
	int	signal;

	bit = 7;
	while (bit >= 0)
	{
		g_acknowledged = 0;
		if ((c >> bit) & 1)
			signal = SIGUSR1;
		else
			signal = SIGUSR2;
		if (kill(pid, signal) == -1)
		{
			ft_putstr_fd("ERROR: failed to send signal\n", 1);
			exit(1);
		}
		while (!g_acknowledged)
			usleep(50);
		bit--;
	}
}

int	main(int argc, char **argv)
{
	pid_t			pid;
	int				i;
	unsigned char	*msg;

	if (argc != 3)
	{
		ft_putstr_fd("ERROR: usage is ./client <PID> <message>\n", 1);
		return (1);
	}
	pid = ft_atoi(argv[1]);
	if (pid <= 0 || pid > 4194304)
	{
		ft_putstr_fd("ERROR: invalid PID\n", 1);
		return (1);
	}
	signal(SIGUSR1, handle_ack);
	msg = (unsigned char *)argv[2];
	i = 0;
	while (msg[i])
	{
		send_char(pid, msg[i]);
		i++;
	}
	return (0);
}
