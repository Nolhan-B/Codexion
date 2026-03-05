#include "codexion.h"

void	wait_cooldown(t_dongle *dongle)
{
	long	now;

	while (1)
	{
		now = get_time_ms();
		if (now >= dongle->cooldown_end)
			break;
		pthread_mutex_unlock(&dongle->mutex);
		usleep(1000);
		pthread_mutex_lock(&dongle->mutex);
	}
}

long	calculate_priority(t_coder *coder)
{
	if (coder->sim->config.scheduler == FIFO)
		return (get_time_ms());
	else
		return (coder->last_compile_start
			+ coder->sim->config.time_to_burnout);
}

void	take_dongle(t_coder *coder, t_dongle *dongle)
{
	long	priority;

	pthread_mutex_lock(&dongle->mutex);
	wait_cooldown(dongle);
	priority = calculate_priority(coder);
	queue_push(dongle->queue, coder, priority);
	while (1)
	{
		if (!queue_is_empty(dongle->queue)
			&& dongle->queue->nodes[0].coder == coder
			&& !dongle->is_taken)
			break;
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
	queue_pop(dongle->queue);
	dongle->is_taken = 1;
	pthread_cond_broadcast(&dongle->cond);
	print_log(coder->sim, coder->id, "has taken a dongle");
	pthread_mutex_unlock(&dongle->mutex);
}

void	release_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->cooldown_end = get_time_ms()
		+ coder->sim->config.dongle_cooldown;
	dongle->is_taken = 0;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}