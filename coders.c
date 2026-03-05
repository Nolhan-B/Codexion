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
	long now;
	long priority;

	pthread_mutex_lock(&dongle->mutex);

	// Attendre la fin du cooldown
	while (1)
	{
		now = get_time_ms();
		if (now >= dongle->cooldown_end)
			break;
		pthread_mutex_unlock(&dongle->mutex);
		usleep(1000);
		pthread_mutex_lock(&dongle->mutex);
	}

	// Calculer la priorité selon FIFO ou EDF
	if (coder->sim->config.scheduler == FIFO)
		priority = get_time_ms();  // FIFO : timestamp actuel
	else
		priority = coder->last_compile_start + coder->sim->config.time_to_burnout;  // EDF : deadline

	// S'inscrire dans la queue
	queue_push(dongle->queue, coder, priority);

	// Attendre d'être le plus prioritaire
	while (1)
	{
		if (!queue_is_empty(dongle->queue) && dongle->queue->nodes[0].coder == coder && !dongle->is_taken)
			break;
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}

	// Retirer de la queue et prendre le dongle
	queue_pop(dongle->queue);
	dongle->is_taken = 1;
	pthread_cond_broadcast(&dongle->cond);
	print_log(coder->sim, coder->id, "has taken a dongle");
	pthread_mutex_unlock(&dongle->mutex);
}

static void release_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->cooldown_end = get_time_ms() + coder->sim->config.dongle_cooldown;
	dongle->is_taken = 0;
	pthread_cond_broadcast(&dongle->cond);  // Réveiller tous les threads en attente
	pthread_mutex_unlock(&dongle->mutex);
}

void *coder_routine(void *arg)
{
    t_coder     *coder;
    t_dongle    *first;
    t_dongle    *second;
    int         single_coder;

    coder = (t_coder *)arg;

	if (coder->sim->config.nb_compiles_required == 0)
		return (NULL);

    single_coder = (coder->sim->config.nb_coders == 1);

    if (!single_coder)
    {
        if (coder->left_dongle < coder->right_dongle)
        {
            first = coder->left_dongle;
            second = coder->right_dongle;
        }
        else
        {
            first = coder->right_dongle;
            second = coder->left_dongle;
        }
    }
    else
    {
        first = coder->left_dongle;
        second = NULL;  // Pas de second dongle
    }

    while (is_running(coder->sim))
    {
        if (!is_running(coder->sim))
            break;

        // Prendre le premier dongle
        take_dongle(coder, first);
        if (!is_running(coder->sim))
        {
            release_dongle(coder, first);
            break;
        }

        // Prendre le second dongle (sauf si 1 coder)
        if (!single_coder)
        {
            take_dongle(coder, second);
            if (!is_running(coder->sim))
            {
                release_dongle(coder, first);
                release_dongle(coder, second);
                break;
            }
        }

        coder->last_compile_start = get_time_ms();
        print_log(coder->sim, coder->id, "is compiling");
        usleep(coder->sim->config.time_to_compile * 1000);

        // Relâcher les dongles
        release_dongle(coder, first);
        if (!single_coder)
            release_dongle(coder, second);

        if (!is_running(coder->sim))
            break;

        print_log(coder->sim, coder->id, "is debugging");
        usleep(coder->sim->config.time_to_debug * 1000);

        if (!is_running(coder->sim))
            break;

        print_log(coder->sim, coder->id, "is refactoring");
        usleep(coder->sim->config.time_to_refactor * 1000);

        coder->compile_count++;
        if (coder->compile_count >= coder->sim->config.nb_compiles_required)
            break;
    }
    return (NULL);
}