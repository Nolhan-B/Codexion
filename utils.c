#include "codexion.h"

int	is_running(t_sim *sim)
{
	int	running;

	pthread_mutex_lock(&sim->mutex);
	running = sim->running;
	pthread_mutex_unlock(&sim->mutex);
	return (running);
}

void	stop_simulation(t_sim *sim)
{
	pthread_mutex_lock(&sim->mutex);
	sim->running = 0;
	pthread_mutex_unlock(&sim->mutex);
}

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	print_log(t_sim *sim, int coder_id, char *msg)
{
	long	timestamp;

	pthread_mutex_lock(&sim->log_mutex);
	timestamp = get_time_ms() - sim->start_time;
	if (is_running(sim))
		printf("%ld %d %s\n", timestamp, coder_id, msg);
	pthread_mutex_unlock(&sim->log_mutex);
}
