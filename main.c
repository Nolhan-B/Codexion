/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbilyj <nbilyj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 11:18:02 by nbilyj            #+#    #+#             */
/*   Updated: 2026/03/06 11:18:10 by nbilyj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **av)
{
	t_sim	sim;
	int		i;

	if (parse_args(ac, av, &sim.config) == -1)
		return (1);
	if (init_simulation(&sim) == -1)
		return (fprintf(stderr, "Error: init failed\n"), 1);
	i = 0;
	while (i < sim.config.nb_coders)
	{
		if (pthread_join(sim.coders[i].thread, NULL) != 0)
			fprintf(stderr, "Warning: pthread_join failed for coder %d\n", i);
		i++;
	}
	if (pthread_join(sim.monitor_thread, NULL) != 0)
		fprintf(stderr, "Warning: pthread_join failed for monitor\n");
	destroy_sim(&sim);
	return (0);
}
