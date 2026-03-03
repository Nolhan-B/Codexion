#include "codexion.h"

t_priority_queue *queue_init(int capacity, t_scheduler scheduler)
{
	t_priority_queue *queue;

	queue = malloc(sizeof(t_priority_queue));
	if (!queue)
		return (NULL);
	queue->nodes = malloc(sizeof(t_queue_node) * capacity);
	if (!queue->nodes)
	{
		free(queue);
		return (NULL);
	}
	queue->size = 0;
	queue->capacity = capacity;
	queue->scheduler = scheduler;
	return (queue);
}

void queue_destroy(t_priority_queue *queue)
{
	if (!queue)
		return;
	if (queue->nodes)
		free(queue->nodes);
	free(queue);
}