#include "codexion.h"

static void	setup_dongles(t_coder *coder, t_dongle **first,
	t_dongle **second, int single)
{
	if (!single)
	{
		if (coder->left_dongle < coder->right_dongle)
		{
			*first = coder->left_dongle;
			*second = coder->right_dongle;
		}
		else
		{
			*first = coder->right_dongle;
			*second = coder->left_dongle;
		}
	}
	else
	{
		*first = coder->left_dongle;
		*second = NULL;
	}
}

static int	take_both_dongles(t_coder *coder, t_dongle *first,
	t_dongle *second, int single)
{
	take_dongle(coder, first);
	if (!is_running(coder->sim))
	{
		release_dongle(coder, first);
		return (0);
	}
	if (!single)
	{
		take_dongle(coder, second);
		if (!is_running(coder->sim))
		{
			release_dongle(coder, first);
			release_dongle(coder, second);
			return (0);
		}
	}
	return (1);
}

static void	compile_phase(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->mutex);
	coder->last_compile_start = get_time_ms();
	coder->compile_count++;
	pthread_mutex_unlock(&coder->sim->mutex);
	print_log(coder->sim, coder->id, "is compiling");
	usleep(coder->sim->config.time_to_compile * 1000);
}

static int	work_cycle(t_coder *coder, t_dongle *first,
	t_dongle *second, int single)
{
	if (!is_running(coder->sim))
		return (0);
	if (!take_both_dongles(coder, first, second, single))
		return (0);
	compile_phase(coder);
	release_dongle(coder, first);
	if (!single)
		release_dongle(coder, second);
	if (!is_running(coder->sim))
		return (0);
	print_log(coder->sim, coder->id, "is debugging");
	usleep(coder->sim->config.time_to_debug * 1000);
	if (!is_running(coder->sim))
		return (0);
	print_log(coder->sim, coder->id, "is refactoring");
	usleep(coder->sim->config.time_to_refactor * 1000);
	if (coder->compile_count >= coder->sim->config.nb_compiles_required)
		return (0);
	return (1);
}

void	*coder_routine(void *arg)
{
	t_coder		*coder;
	t_dongle	*first;
	t_dongle	*second;
	int			single;

	coder = (t_coder *)arg;
	if (coder->sim->config.nb_compiles_required == 0)
		return (NULL);
	single = (coder->sim->config.nb_coders == 1);
	setup_dongles(coder, &first, &second, single);
	while (is_running(coder->sim))
	{
		if (!work_cycle(coder, first, second, single))
			break;
	}
	return (NULL);
}