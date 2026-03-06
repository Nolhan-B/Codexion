/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbilyj <nbilyj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 11:18:16 by nbilyj            #+#    #+#             */
/*   Updated: 2026/03/06 11:18:17 by nbilyj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		i;
	long	now;
	int		all_done;

	sim = (t_sim *)arg;
	while (is_running(sim))
	{
		usleep(10000);
		now = get_time_ms();
		all_done = 1;
		i = -1;
		while (++i < sim->config.nb_coders)
		{
			if (check_coder_deadline(sim, i, now) == -1)
				return (NULL);
			if (check_coder_deadline(sim, i, now) == 0)
				all_done = 0;
		}
		if (all_done)
			break ;
	}
	stop_simulation(sim);
	return (NULL);
}
