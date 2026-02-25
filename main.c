#include "codexion.h"

int main(int ac, char **av)
{
	t_config	config;

	if (ac != 9)
	{
		fprintf(stderr, "Invalid number of arguments.\n End of Program\n");
		return (1);
	}
	if (parse_args(ac, av, &config) == -1)
		return (1);
	return (0);
}