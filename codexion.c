#include "codexion.h"

static int	parse_scheduler(char *str, t_config *config)
{
	if (strcmp(str, "fifo") == 0)
		config->scheduler = FIFO;
	else if (strcmp(str, "edf") == 0)
		config->scheduler = EDF;
	else
		return (-1);
	return (0);
}

static int	parse_int_arg(char *str)
{
	int	i;
	int	result;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (-1);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		i++;
	}
	result = atoi(str);
	if (result < 0)
		return (-1);
	return (result);
}

static int	parse_all_times(char **av, t_config *config)
{
	config->time_to_burnout = parse_int_arg(av[2]);
	if (config->time_to_burnout < 0)
		return (-1);
	config->time_to_compile = parse_int_arg(av[3]);
	if (config->time_to_compile < 0)
		return (-1);
	config->time_to_debug = parse_int_arg(av[4]);
	if (config->time_to_debug < 0)
		return (-1);
	config->time_to_refactor = parse_int_arg(av[5]);
	if (config->time_to_refactor < 0)
		return (-1);
	return (0);
}

int	parse_args(int ac, char **av, t_config *config)
{
	if (ac != 9)
		return (fprintf(stderr, "Error: wrong number of arguments\n"), -1);
	config->nb_coders = parse_int_arg(av[1]);
	if (config->nb_coders <= 0)
		return (fprintf(stderr, "Error: invalid number_of_coders\n"), -1);
	if (parse_all_times(av, config) == -1)
		return (fprintf(stderr, "Error: invalid time argument\n"), -1);
	config->nb_compiles_required = parse_int_arg(av[6]);
	if (config->nb_compiles_required < 0)
		return (fprintf(stderr, "Error: invalid compiles_required\n"), -1);
	config->dongle_cooldown = parse_int_arg(av[7]);
	if (config->dongle_cooldown < 0)
		return (fprintf(stderr, "Error: invalid dongle_cooldown\n"), -1);
	if (parse_scheduler(av[8], config) == -1)
		return (fprintf(stderr, "Error: scheduler must be fifo or edf\n"), -1);
	return (0);
}