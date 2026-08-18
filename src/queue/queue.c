/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 17:11:57 by hgawhari          #+#    #+#             */
/*   Updated: 2026/08/18 13:25:14 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
 * Initialize an empty queue and assign its comparison function.
 * The comparison function determines the priority order of requests.
 */
void	queue_init(t_queue *q, int (*cmp)(t_task, t_task))
{
	q->head = NULL;
	q->size = 0;
	q->cmp = cmp;
}

/*
 * Return the request at the front of the queue.
 * The front request has the highest priority according to the scheduler.
 */
t_task	queue_top(t_queue *q)
{
	return (q->head->req);
}

/*
 * Add a request to the queue in scheduler priority order.
 * FIFO or EDF determines where the new request is inserted.
 */
void	queue_push(t_queue *q, t_task req)
{
	t_node	*new_node;
	t_node	*previous;
	t_node	*current;

	new_node = malloc(sizeof(t_node));
	if (!new_node)
		return ;
	new_node->req = req;
	new_node->next = NULL;
	previous = NULL;
	current = q->head;
	while (current && q->cmp(current->req, req) < 0)
	{
		previous = current;
		current = current->next;
	}
	new_node->next = current;
	if (previous)
		previous->next = new_node;
	else
		q->head = new_node;
	q->size++;
}

/*
 * Remove the highest-priority request from the queue.
 * The removed node is freed and the queue size is updated.
 */
void	queue_pop(t_queue *q)
{
	t_node	*removed_node;

	if (!q || !q->head)
		return ;
	removed_node = q->head;
	q->head = removed_node->next;
	free(removed_node);
	q->size--;
}

/*
 * Remove and free all nodes in the queue.
 * The queue is reset to an empty state.
 */
void	queue_destroy(t_queue *q)
{
	t_node	*current;
	t_node	*next_node;

	current = q->head;
	while (current)
	{
		next_node = current->next;
		free(current);
		current = next_node;
	}
	q->head = NULL;
	q->size = 0;
}
