/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbilyj <nbilyj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 11:18:41 by nbilyj            #+#    #+#             */
/*   Updated: 2026/03/06 11:18:42 by nbilyj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	queue_is_empty(t_priority_queue *queue)
{
	return (queue->size == 0);
}

static void	swap_nodes(t_queue_node *a, t_queue_node *b)
{
	t_queue_node	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static int	is_higher_priority(t_priority_queue *queue, int i, int j)
{
	if (queue->nodes[i].priority == queue->nodes[j].priority)
		return (queue->nodes[i].coder->id > queue->nodes[j].coder->id);
	return (queue->nodes[i].priority < queue->nodes[j].priority);
}

void	bubble_up(t_priority_queue *queue, int index)
{
	int	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (is_higher_priority(queue, index, parent))
		{
			swap_nodes(&queue->nodes[index], &queue->nodes[parent]);
			index = parent;
		}
		else
			break ;
	}
}

void	bubble_down(t_priority_queue *queue, int index)
{
	int	left;
	int	right;
	int	smallest;

	while (1)
	{
		left = 2 * index + 1;
		right = 2 * index + 2;
		smallest = index;
		if (left < queue->size && is_higher_priority(queue, left, smallest))
			smallest = left;
		if (right < queue->size
			&& is_higher_priority(queue, right, smallest))
			smallest = right;
		if (smallest != index)
		{
			swap_nodes(&queue->nodes[index], &queue->nodes[smallest]);
			index = smallest;
		}
		else
			break ;
	}
}
