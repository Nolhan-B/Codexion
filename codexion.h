#ifndef CODEXION_H
# define CODEXION_H

typedef enum e_scheduler
{
	FIFO,
	EDF
}	t_scheduler;

typedef struct s_config
{
	int 		coders_count;
	int 		tt_burnout;
	int			tt_compile;
	int 		tt_debug;
	int 		tt_refactor;
	int 		nb_compiles_required;
	int 		dongle_cooldown;
	t_scheduler	scheduler;
}	t_config;

#endif