/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   acquire_dongles.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 15:58:16 by hgawhari          #+#    #+#             */
/*   Updated: 2026/08/22 12:31:27 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
 * Create a request for both dongles and add it to their wait queues.
 * The request uses the coder's deadline and a global arrival order
 * so that both dongles use the same scheduling information.
 */
static void	queue_pair_request(t_coder *coder, t_data *data, t_dongle *first,
		t_dongle *second)
{
	t_task	req;

	req.coder_id = coder->id;
	pthread_mutex_lock(&coder->mutex);
	req.deadline = coder->last_compile_ms + data->time_to_burnout;
	pthread_mutex_unlock(&coder->mutex);
	pthread_mutex_lock(&data->counter_mutex);
	req.arrival_order = data->request_counter++;
	pthread_mutex_unlock(&data->counter_mutex);
	pthread_mutex_lock(&first->mutex);
	queue_push(&first->wait_queue, req);
	pthread_mutex_unlock(&first->mutex);
	pthread_mutex_lock(&second->mutex);
	queue_push(&second->wait_queue, req);
	pthread_mutex_unlock(&second->mutex);
}

/*
 * Try to atomically acquire both dongles.
 * The coder must be first in both queues and both dongles
 * must be available and ready.
 */
static bool	try_take_pair(t_coder *coder, t_dongle *first, t_dongle *second,
		t_data *data)
{
	pthread_mutex_lock(&first->mutex);
	pthread_mutex_lock(&second->mutex);
	if (!can_take_pair(coder, first, second, data))
	{
		pthread_mutex_unlock(&second->mutex);
		pthread_mutex_unlock(&first->mutex);
		return (false);
	}
	queue_pop(&first->wait_queue);
	queue_pop(&second->wait_queue);
	first->available = false;
	second->available = false;
	pthread_mutex_unlock(&second->mutex);
	pthread_mutex_unlock(&first->mutex);
	return (true);
}

/*
 * Wait for the dongle that is currently preventing the pair
 * from being acquired.
 */
static void	wait_for_pair(t_coder *coder, t_data *data, t_dongle *first,
		t_dongle *second)
{
	pthread_mutex_lock(&first->mutex);
	if (!can_take(first, coder->id) || !is_dongle_ready(first, data))
	{
		pthread_mutex_unlock(&first->mutex);
		wait_for_dongle(first, coder, data);
		return ;
	}
	pthread_mutex_unlock(&first->mutex);
	wait_for_dongle(second, coder, data);
}

/*
 * Queue the coder's request and wait until both dongles
 * can be acquired or the simulation stops.
 */
static bool	get_dongle_pair(t_coder *coder, t_data *data, t_dongle *first,
		t_dongle *second)
{
	queue_pair_request(coder, data, first, second);
	while (simulation_is_running(data))
	{
		if (try_take_pair(coder, first, second, data))
			return (true);
		wait_for_pair(coder, data, first, second);
	}
	return (false);
}

/*
 * Acquire both dongles required by the coder.
 * The coder must successfully acquire both before compilation starts.
 */
bool	acquire_dongles(t_coder *coder, t_data *data)
{
	t_dongle	*first;
	t_dongle	*second;

	get_left_right_dongles(coder, &first, &second);
	if (first == second)
		return (false);
	if (!get_dongle_pair(coder, data, first, second))
		return (false);
	log_event(data, coder->id, "has taken a dongle");
	log_event(data, coder->id, "has taken a dongle");
	return (true);
}
