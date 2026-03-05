#include "codexion.h"

int	check_coder_deadline(t_sim *sim, int i, long now)
{
	long	deadline;
	long	last_compile;
	int		compile_count;
	int		id;

	pthread_mutex_lock(&sim->mutex);
	last_compile = sim->coders[i].last_compile_start;
	compile_count = sim->coders[i].compile_count;
	id = sim->coders[i].id;
	pthread_mutex_unlock(&sim->mutex);
	if (compile_count >= sim->config.nb_compiles_required)
		return (1);
	if (last_compile == 0)
		deadline = sim->start_time + sim->config.time_to_burnout;
	else
		deadline = last_compile + sim->config.time_to_burnout;
	if (now > deadline)
	{
		print_log(sim, id, "burned out");
		stop_simulation(sim);
		return (-1);
	}
	return (0);
}