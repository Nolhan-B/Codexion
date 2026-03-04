/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbilyj <nbilyj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 10:03:43 by nbilyj            #+#    #+#             */
/*   Updated: 2026/03/04 10:17:55 by nbilyj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_higher_priority(t_priority_queue *queue, int i, int j)
{
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
		if (right < queue->size && is_higher_priority(queue, right, smallest))
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
