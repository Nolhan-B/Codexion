#include "codexion.h"

int create_threads(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.nb_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL, coder_routine, &sim->coders[i]))
			return (-1);
		i++;
	}
	if (pthread_create(&sim->monitor_thread, NULL, monitor_routine, sim))
		return (-1);
	return (0);
}