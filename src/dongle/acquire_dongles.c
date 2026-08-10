/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   acquire_dongles.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 15:58:16 by hgawhari          #+#    #+#             */
/*   Updated: 2026/08/09 22:17:21 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int can_take(t_dongle *dongle, unsigned int coder_id)
{
	t_request  top;

	if (dongle->wait_queue.size == 0)
		return (0);
	top = queue_peek(&dongle->wait_queue);
	if (top.coder_id != coder_id)
		return (0);
	if (!dongle->available)
		return (0);
	return (1);
}

static void wait_cooldown(t_dongle *dongle, t_data *data)
{
	struct timeval tv;
	struct timespec ts;
	unsigned long elapsed;
	unsigned long remaining;

	elapsed = get_time_ms() - dongle->last_release_ms;
	if (elapsed >= data->dongle_cooldown)
		return ;
	remaining = data->dongle_cooldown - elapsed;
	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec + remaining / 1000;
	ts.tv_nsec = (tv.tv_usec * 1000) + (remaining % 1000) * 1000000;
	pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
}

int cooldown_ok(t_dongle *dongle, t_data *data)
{
	unsigned long elapsed = get_time_ms() - dongle->last_release_ms;
	return (elapsed >= data->dongle_cooldown);
}

static void get_dongle(t_coder *coder, t_data *data, t_dongle *dongle)
{
	t_request req;

	req.coder_id = coder->id;
	req.deadline = coder->last_compile_ms + data->time_to_burnout;
	pthread_mutex_lock(&data->counter_mutex);
	req.arrival_order = data->request_counter++;
	pthread_mutex_unlock(&data->counter_mutex);
	pthread_mutex_lock(&dongle->mutex);
	push_to_queue(&dongle->wait_queue, req);
	while (is_running(data) && (!can_take(dongle, coder->id) || !cooldown_ok(dongle, data)))
	{
		if (can_take(dongle, coder->id))
			wait_cooldown(dongle, data);
		else
			pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
	if (is_running(data))
	{
		pop_queue(&dongle->wait_queue);
		dongle->available =false;
	}
	pthread_mutex_unlock(&dongle->mutex);

}

void	acquire_dongles(t_coder *coder,  t_data *data)
{
	t_dongle *first;
	t_dongle *second;

	get_dongle_queue(coder, &first, &second);
	get_dongle(coder, data, first);
	if (! is_running(data))
		return ;
	log_action(data, coder->id, "has taken a dongle");
	if (first == second)
		return ;
	get_dongle(coder, data, second);
	if (! is_running(data))
		return ;
	log_action(data, coder->id, "has taken a dongle");
}
