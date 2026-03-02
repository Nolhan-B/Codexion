#include "codexion.h"

int main(int ac, char **av)
{
	t_sim sim;

	if (parse_args(ac, av, &sim.config) == -1)
		return (1);
	if (init_simulation(&sim) == -1)
		return (fprintf(stderr, "Error: init failed\n"), 1);

	printf("Simulation initialized with %d coders\n", sim.config.nb_coders);

	destroy_sim(&sim);  // ← Teste le cleanup
	printf("Simulation destroyed\n");
	return (0);
}