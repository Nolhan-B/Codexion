#include "codexion.h"

void *coder_routine(void *arg)
{
	t_coder *coder;

	coder = (t_coder *)arg;
	printf("Coder %d started\n", coder->id);

	// TODO: boucle principale

	return (NULL);
}