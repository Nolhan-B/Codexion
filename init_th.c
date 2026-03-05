#include "codexion.h"

static int	create_coder_threads(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.nb_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL,
				coder_routine, &sim->coders[i]))
			return (-1);
		i++;
	}
	return (0);
}

int	create_threads(t_sim *sim)
{
	if (create_coder_threads(sim) == -1)
		return (-1);
	if (pthread_create(&sim->monitor_thread, NULL, monitor_routine, sim))
		return (-1);
	return (0);
}