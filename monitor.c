#include "codexion.h"

void *monitor_routine(void *arg)
{
	t_sim *sim;
	int i;
	long now;
	long deadline;
	int all_done;
	long last_compile;
	int compile_count;

	sim = (t_sim *)arg;
	while (is_running(sim))
	{
		usleep(10000);
		now = get_time_ms();
		all_done = 1;

		i = 0;
		while (i < sim->config.nb_coders)
		{
			int id;

			pthread_mutex_lock(&sim->mutex);
			last_compile = sim->coders[i].last_compile_start;
			compile_count = sim->coders[i].compile_count;
			id = sim->coders[i].id;
			pthread_mutex_unlock(&sim->mutex);

			if (compile_count < sim->config.nb_compiles_required)
			{
				all_done = 0;

				if (last_compile == 0)
					deadline = sim->start_time + sim->config.time_to_burnout;
				else
					deadline = last_compile + sim->config.time_to_burnout;

				if (now > deadline)
				{
					print_log(sim, id, "burned out");
					stop_simulation(sim);
					return (NULL);
				}
			}
			i++;
		}
		if (all_done)
		{
			stop_simulation(sim);
			return (NULL);
		}
	}
	return (NULL);
}