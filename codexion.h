#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef enum e_scheduler
{
	FIFO,
	EDF
}	t_scheduler;

typedef struct s_config
{
	int			nb_coders;
	int			time_to_burnout;
	int			time_to_compile;
	int			time_to_debug;
	int			time_to_refactor;
	int			nb_compiles_required;
	int			dongle_cooldown;
	t_scheduler	scheduler;
}	t_config;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	long			cooldown_end;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	long			last_compile_start;
	int				compile_count;
	struct s_sim	*sim;
}	t_coder;

typedef struct s_sim
{
	t_coder			*coders;
	t_dongle		*dongles;
	t_config		config;
	int				running;
	pthread_mutex_t	mutex;
	long			start_time;
	pthread_mutex_t	log_mutex;
}	t_sim;

/* codexion.c */
int		parse_args(int ac, char **av, t_config *config);

/* init.c */
void	destroy_initiated_mutex(t_sim *sim, int last_done);
int		init_dongles(t_sim *sim);
void	init_coders(t_sim *sim);
int		init_simulation(t_sim *sim);

#endif