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

static void take_dongle(t_coder *coder, t_dongle *dongle)
{
	long	now;

	pthread_mutex_lock(&dongle->mutex);
	while (1)
	{
		now = get_time_ms();
		if (now >= dongle->cooldown_end)
			break;
		pthread_mutex_unlock(&dongle->mutex);
		usleep(1000);
		pthread_mutex_lock(&dongle->mutex);
	}
	print_log(coder->sim, coder->id, "has taken a dongle");
}

static void release_dongle(t_coder *coder, t_dongle *dongle)
{
	dongle->cooldown_end = get_time_ms() + coder->sim->config.dongle_cooldown;
	pthread_mutex_unlock(&dongle->mutex);
}

void *coder_routine(void *arg)
{
	t_coder *coder;

	coder = (t_coder *)arg;
	printf("Coder %d started\n", coder->id);
	while (coder->sim->running)
	{
		// Prendre les 2 dongles
		take_dongle(coder, coder->left_dongle);
		take_dongle(coder, coder->right_dongle);

		coder->last_compile_start = get_time_ms();
		print_log(coder->sim, coder->id, "is compiling");
		usleep(coder->sim->config.time_to_compile * 1000);

		// Relâcher les 2 dongles
		release_dongle(coder, coder->left_dongle);
		release_dongle(coder, coder->right_dongle);

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