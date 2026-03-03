#include "codexion.h"

int queue_is_empty(t_priority_queue *queue)
{
	return (queue->size == 0);
}

static void swap_nodes(t_queue_node *a, t_queue_node *b)
{
	t_queue_node tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static int is_higher_priority(t_priority_queue *queue, int i, int j)
{
	return (queue->nodes[i].priority < queue->nodes[j].priority);
}

static void bubble_up(t_priority_queue *queue, int index)
{
	int parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (is_higher_priority(queue, index, parent))
		{
			swap_nodes(&queue->nodes[index], &queue->nodes[parent]);
			index = parent;
		}
		else
			break;
	}
}

static void bubble_down(t_priority_queue *queue, int index)
{
	int left;
	int right;
	int smallest;

	while (1)
	{
		left = 2 * index + 1;
		right = 2 * index + 2;
		smallest = index;

		if (left < queue->size && is_higher_priority(queue, left, smallest))
			smallest = left;
		if (right < queue->size && is_higher_priority(queue, right, smallest))
			smallest = right;

		if (smallest != index)
		{
			swap_nodes(&queue->nodes[index], &queue->nodes[smallest]);
			index = smallest;
		}
		else
			break;
	}
}

int queue_push(t_priority_queue *queue, t_coder *coder, long priority)
{
	if (queue->size >= queue->capacity)
		return (-1);  // Queue pleine

	queue->nodes[queue->size].coder = coder;
	queue->nodes[queue->size].priority = priority;
	bubble_up(queue, queue->size);
	queue->size++;
	return (0);
}

t_coder *queue_pop(t_priority_queue *queue)
{
	t_coder *coder;

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