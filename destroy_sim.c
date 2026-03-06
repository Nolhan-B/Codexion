/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_sim.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbilyj <nbilyj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 10:57:28 by nbilyj            #+#    #+#             */
/*   Updated: 2026/03/06 10:57:29 by nbilyj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	destroy_sim(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.nb_coders)
	{
		pthread_cond_destroy(&sim->dongles[i].cond);
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		queue_destroy(sim->dongles[i].queue);
		i++;
	}
	free(sim->coders);
	free(sim->dongles);
	pthread_mutex_destroy(&sim->mutex);
	pthread_mutex_destroy(&sim->log_mutex);
}
