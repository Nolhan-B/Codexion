#include "codexion.h"

int main(int ac, char **av)
{
	t_sim sim;
	int i;

	if (parse_args(ac, av, &sim.config) == -1)
		return (1);
	if (init_simulation(&sim) == -1)
		return (fprintf(stderr, "Error: init failed\n"), 1);

	printf("Simulation initialized with %d coders\n", sim.config.nb_coders);
	i = 0;
	while (i < sim.config.nb_coders)
	{
		pthread_join(sim.coders[i].thread, NULL);
		i++;
	}
	destroy_sim(&sim);
	printf("Simulation destroyed\n");
	return (0);
}