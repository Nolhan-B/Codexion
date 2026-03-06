/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbilyj <nbilyj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 11:09:31 by nbilyj            #+#    #+#             */
/*   Updated: 2026/03/06 11:09:34 by nbilyj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	destroy_initiated_mutex(t_sim *sim, int last_done)
{
	int	j;

	j = 0;
	while (j < last_done)
	{
		pthread_mutex_destroy(&sim->dongles[j].mutex);
		pthread_cond_destroy(&sim->dongles[j].cond);
		queue_destroy(sim->dongles[j].queue);
		j++;
	}
}
