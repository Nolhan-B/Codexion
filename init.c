#include "codexion.h"

// typedef struct s_dongle
// {
// 	pthread_mutex_t	mutex;
// 	pthread_cond_t	cond;
// 	long			cooldown_end;
// }	t_dongle;

void  destroy_initiated_mutex(t_sim *sim, int last_done)
{
	int	j;

	j = 0;
	while (j < last_done)
	{
		pthread_mutex_destroy(&sim->dongles[j].mutex);
		pthread_cond_destroy(&sim->dongles[j].cond);
		j++;
	}
}

int init_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.nb_coders)
	{
		if (pthread_mutex_init(&sim->dongles[i].mutex, NULL) != 0)
		{
			destroy_initiated_mutex(sim, i);
			return (-1);
		}
		if (pthread_cond_init(&sim->dongles[i].cond, NULL) != 0)
		{
			pthread_mutex_destroy(&sim->dongles[i].mutex);
			destroy_initiated_mutex(sim, i);
			return (-1);
		}
		sim->dongles[i].cooldown_end = 0;
		i++;
	}
	return (0);
}

int init_simulation(t_sim *sim)
{
	sim->coders = malloc(sizeof(t_coder) * sim->config.nb_coders);
	if (!sim->coders)
		return (-1);
	sim->dongles = malloc(sizeof(t_dongle) * sim->config.nb_coders);
	if (!sim->dongles)
		return (free(sim->coders), -1);
	if (pthread_mutex_init(&sim->mutex, NULL) != 0)
	{
		free(sim->coders);
		free(sim->dongles);
		return (-1);
	}
	if (pthread_mutex_init(&sim->log_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->mutex);
		free(sim->coders);
		free(sim->dongles);
		return (-1);
	}
	if (init_dongles(sim) == -1)
	{
		pthread_mutex_destroy(&sim->log_mutex);
		pthread_mutex_destroy(&sim->mutex);
		free(sim->coders);
		free(sim->dongles);
		return (-1);
	}
	return (0);
}