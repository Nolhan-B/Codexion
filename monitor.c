#include "codexion.h"

void *monitor_routine(void *arg)
{
	t_sim	*sim;
	int		i;
	long	now;
	long	deadline;

	sim = (t_sim *)arg;
	while (sim->running)
	{
		usleep(10000);
		now = get_time_ms();
		i = 0;
		while (i < sim->config.nb_coders)
		{
			pthread_mutex_lock(&sim->coders[i].mutex);
			if (sim->coders[i].last_compile_start == 0)
				deadline = sim->start_time + sim->config.time_to_burnout;
			else
				deadline = sim->coders[i].last_compile_start + sim->config.time_to_burnout;
			pthread_mutex_unlock(&sim->coders[i].mutex);
			if (now > deadline)
			{
				print_log(sim, sim->coders[i].id, "burned out");
				sim->running = 0;
				return (NULL);
			}
			i++;
		}
	}
	return (NULL);
}