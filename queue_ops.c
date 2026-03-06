/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_ops.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbilyj <nbilyj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 10:56:33 by nbilyj            #+#    #+#             */
/*   Updated: 2026/03/06 11:19:06 by nbilyj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	queue_push(t_priority_queue *q, t_coder *c, long p)
{
	if (q->size >= q->capacity)
		return (-1);
	q->nodes[q->size].coder = c;
	q->nodes[q->size].priority = p;
	bubble_up(q, q->size);
	q->size++;
	return (0);
}

t_coder	*queue_pop(t_priority_queue *queue)
{
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
