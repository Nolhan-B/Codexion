#include "codexion.h"

int	queue_push(t_priority_queue *queue, t_coder *coder, long priority)
{
	extern void	bubble_up(t_priority_queue *queue, int index);

	if (queue->size >= queue->capacity)
		return (-1);
	queue->nodes[queue->size].coder = coder;
	queue->nodes[queue->size].priority = priority;
	bubble_up(queue, queue->size);
	queue->size++;
	return (0);
}

t_coder	*queue_pop(t_priority_queue *queue)
{
	extern void		bubble_down(t_priority_queue *queue, int index);
	extern int		queue_is_empty(t_priority_queue *queue);
	t_coder			*coder;

	if (queue_is_empty(queue))
		return (NULL);
	coder = queue->nodes[0].coder;
	queue->size--;
	if (queue->size > 0)
	{
		queue->nodes[0] = queue->nodes[queue->size];
		bubble_down(queue, 0);
	}
	return (coder);
}