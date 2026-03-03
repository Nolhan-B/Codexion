#include "codexion.h"

// static int check_completion(t_sim *sim)
// {
// 	int	i;
//
// 	pthread_mutex_lock(&sim->mutex);
// 	i = 0;
// 	while (i < sim->config.nb_coders)
// 	{
// 		if (sim->coders[i].compile_count < sim->config.nb_compiles_required)
// 		{
// 			pthread_mutex_unlock(&sim->mutex);
// 			return (0);
// 		}
// 		i++;
// 	}
// 	sim->running = 0;
// 	pthread_mutex_unlock(&sim->mutex);
// 	return (1);
// }

void *coder_routine(void *arg)
{
	t_coder *coder;

	coder = (t_coder *)arg;
	printf("Coder %d started\n", coder->id);
	while (coder->sim->running)
	{

		print_log(coder->sim, coder->id, "is compiling");
		usleep(coder->sim->config.time_to_compile * 1000);  // ms → µs

		print_log(coder->sim, coder->id, "is debugging");
		usleep(coder->sim->config.time_to_debug * 1000);

		print_log(coder->sim, coder->id, "is refactoring");
		usleep(coder->sim->config.time_to_refactor * 1000);

		coder->compile_count++;

		if (coder->compile_count >= coder->sim->config.nb_compiles_required)
			break;
	}

	return (NULL);
}