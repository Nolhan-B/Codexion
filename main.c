#include "codexion.h"

int main(int ac, char **av)
{
	t_config	config;

	if (parse_args(ac, av, &config) == -1)
		return (1);
	return (0);
}