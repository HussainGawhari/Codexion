/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 12:24:20 by hgawhari          #+#    #+#             */
/*   Updated: 2026/08/22 12:31:19 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
 * Check whether the coder is currently first in the dongle queue
 * and whether the dongle is available.
 */
int	can_take(t_dongle *dongle, unsigned int coder_id)
{
	t_task	top;

	if (dongle->wait_queue.size == 0)
		return (0);
	top = queue_top(&dongle->wait_queue);
	return (top.coder_id == coder_id && dongle->available);
}

/*
 * Wait until the dongle cooldown period expires.
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
	ts.tv_nsec = tv.tv_usec * 1000 + (remaining % 1000) * 1000000;
	pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
}

/*
 * Check whether the dongle cooldown period has expired.
 */
int	is_dongle_ready(t_dongle *dongle, t_data *data)
{
	return (get_time_ms() - dongle->last_release_ms >= data->dongle_cooldown);
}

/*
 * Check whether both dongles can be taken by the coder.
 * The coder must be first in both queues and both dongles
 * must be available and ready.
 */
int	can_take_pair(t_coder *coder, t_dongle *first, t_dongle *second,
		t_data *data)
{
	return (can_take(first, coder->id) && can_take(second, coder->id)
		&& is_dongle_ready(first, data) && is_dongle_ready(second, data));
}

/*
 * Wait until the dongle becomes available and its cooldown expires.
 * A condition wait is used when another coder owns the dongle.
 */
void	wait_for_dongle(t_dongle *dongle, t_coder *coder, t_data *data)
{
	pthread_mutex_lock(&dongle->mutex);
	while (simulation_is_running(data) && (!can_take(dongle, coder->id)
			|| !is_dongle_ready(dongle, data)))
	{
		if (can_take(dongle, coder->id))
			wait_cooldown(dongle, data);
		else
			pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
	pthread_mutex_unlock(&dongle->mutex);
}
