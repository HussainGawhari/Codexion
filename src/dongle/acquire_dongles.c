/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   acquire_dongles.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 15:58:16 by hgawhari          #+#    #+#             */
/*   Updated: 2026/08/20 12:45:17 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
 * Check whether this coder is first in the waiting queue.
 * The dongle must also be available before it can be acquired.
 */

static int	can_take(t_dongle *dongle, unsigned int coder_id)
{
	t_task	top;

	if (dongle->wait_queue.size == 0)
		return (0);
	top = queue_top(&dongle->wait_queue);
	if (top.coder_id != coder_id)
		return (0);
	if (!dongle->available)
		return (0);
	return (1);
}

/*
 * Wait for the dongle cooldown period to expire.
 * Uses a timed condition wait instead of continuously polling.
 */

static void	wait_cooldown(t_dongle *dongle, t_data *data)
{
	struct timeval	tv;
	struct timespec	ts;
	unsigned long	elapsed;
	unsigned long	remaining;

	elapsed = get_time_ms() - dongle->last_release_ms;
	if (elapsed >= data->dongle_cooldown)
		return ;
	remaining = data->dongle_cooldown - elapsed;
	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec + remaining / 1000;
	ts.tv_nsec = (tv.tv_usec * 1000) + (remaining % 1000) * 1000000;
	pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
}

//static void	wait_cooldown(t_dongle *dongle, t_data *data)
//{
//	struct timeval	tv;
//	struct timespec	ts;
//	unsigned long	elapsed;
//	unsigned long	remaining;

//	elapsed = get_time_ms() - dongle->last_release_ms;
//	if (elapsed >= data->dongle_cooldown)
//		return ;
//	remaining = data->dongle_cooldown - elapsed;
//	gettimeofday(&tv, NULL);
//	ts.tv_sec = tv.tv_sec + remaining / 1000;
//	ts.tv_nsec = tv.tv_usec * 1000
//		+ (remaining % 1000) * 1000000;
//	if (ts.tv_nsec >= 1000000000L)
//	{
//		ts.tv_sec++;
//		ts.tv_nsec -= 1000000000L;
//	}
//	pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
//}

/*
 * Create and enqueue the coder's dongle request.
 * Wait until the scheduler selects the coder and the dongle is ready.
 */

int	is_dongle_ready(t_dongle *dongle, t_data *data)
{
	unsigned long	elapsed;

	elapsed = get_time_ms() - dongle->last_release_ms;
	return (elapsed >= data->dongle_cooldown);
}

/*
 * Create and enqueue the coder's dongle request.
 * Wait until the scheduler selects the coder and the dongle is ready.
 * Create a request, put it into the scheduler queue, wait for permission,
	then mark the dongle as occupied.
 */

static void	get_dongle(t_coder *coder, t_data *data, t_dongle *dongle)
{
	t_task	req;

	req.coder_id = coder->id;
	req.deadline = coder->last_compile_ms + data->time_to_burnout;
	pthread_mutex_lock(&data->counter_mutex);
	req.arrival_order = data->request_counter++;
	pthread_mutex_unlock(&data->counter_mutex);
	
	pthread_mutex_lock(&coder->mutex);
	coder->last_compile_ms = get_time_ms();
	pthread_mutex_unlock(&coder->mutex);
	
	pthread_mutex_lock(&dongle->mutex);
	queue_push(&dongle->wait_queue, req);
	while (simulation_is_running(data) && (!can_take(dongle, coder->id)
			|| !is_dongle_ready(dongle, data)))
	{
		if (can_take(dongle, coder->id))
			wait_cooldown(dongle, data);
		else
			pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
	if (simulation_is_running(data))
	{
		queue_pop(&dongle->wait_queue);
		dongle->available = false;
	}
	pthread_mutex_unlock(&dongle->mutex);
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
	get_dongle(coder, data, first);
	if (!simulation_is_running(data))
		return (false);
	get_dongle(coder, data, second);
	if (!simulation_is_running(data))
	{
		release_dongles_for_coder(coder);
		return (false);
	}
	log_event(data, coder->id, "has taken a dongle");
	log_event(data, coder->id, "has taken a dongle");
	return (true);
}
