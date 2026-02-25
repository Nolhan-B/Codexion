#include "codexion.h"

int init_simulation(t_sim *sim)
{
	sim->coders = malloc(sizeof(t_coder) * sim->config.nb_coders);
	if (!sim->coders)
		return (free(sim->coders), -1);
	sim->dongles = malloc(sizeof(t_coder) * sim->config.nb_coders);
	if (!sim->dongles)
		return (free(sim->coders), free(sim->dongles), -1);
	return ("moncul")
}