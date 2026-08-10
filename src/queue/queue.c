/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 17:11:57 by hgawhari          #+#    #+#             */
/*   Updated: 2026/08/10 19:35:30 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	queue_init(t_queue *q, int (*cmp)(t_task, t_task))
{
	q->head = NULL;
	q->size = 0;
	q->cmp = cmp;
}

t_task	queue_top(t_queue *q)
{
	return (q->head->req);
}

void	queue_push(t_queue *q, t_task req)
{
	t_node	*n;
	t_node	*prev;
	t_node	*curr;

	n = malloc(sizeof(t_node));
	if (!n)
		return ;
	n->req = req;
	n->next = NULL;
	prev = NULL;
	curr = q->head;
	while (curr && q->cmp(curr->req, req) < 0)
	{
		prev = curr;
		curr = curr->next;
	}
	n->next = curr;
	if (prev)
		prev->next = n;
	else
		q->head = n;
	q->size++;
}

void	queue_pop(t_queue *q)
{
	t_node	*old;

	if (!q || !q->head)
		return ;
	old = q->head;
	q->head = old->next;
	free(old);
	q->size--;
}

void	queue_destroy(t_queue *q)
{
	t_node	*curr;
	t_node	*next;

	curr = q->head;
	while (curr)
	{
		next = curr->next;
		free(curr);
		curr = next;
	}
	q->head = NULL;
	q->size = 0;
}
