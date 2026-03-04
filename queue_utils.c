/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbilyj <nbilyj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 10:07:36 by nbilyj            #+#    #+#             */
/*   Updated: 2026/03/04 10:12:42 by nbilyj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	queue_is_empty(t_priority_queue *queue)
{
	return (queue->size == 0);
}

void	swap_nodes(t_queue_node *a, t_queue_node *b)
{
	t_queue_node	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

t_coder	*queue_pop(t_priority_queue *queue)
{
	t_coder	*coder;

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


int	queue_push(t_priority_queue *q, t_coder *c, long pr)
{
	if (q->size >= q->capacity)
		return (-1);
	q->nodes[q->size].coder = c;
	q->nodes[q->size].priority = pr;
	bubble_up(q, q->size);
	q->size++;
	return (0);
}