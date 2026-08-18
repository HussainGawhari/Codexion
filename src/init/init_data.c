/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 16:06:33 by hgawhari          #+#    #+#             */
/*   Updated: 2026/08/18 13:38:28 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
 * This function prepares coders, dongles, mutex,
	and initialize values before thread starts.
 * log_mutex → prevents multiple threads from writing logs
  at the same time
 * simulation_mutex → protects the shared running state.

	* counter_mutex → protects request_counter when multiple coder
	threads create requests.
 * request_counter = 0 → starts the request numbering from zero.
 * running = false → indicates that the simulation has not started yet.
 */
void	init_data(t_data *data)
{
	init_dongles(data);
	init_coders(data);
	pthread_mutex_init(&data->log_mutex, NULL);
	pthread_mutex_init(&data->simulation_mutex, NULL);
	pthread_mutex_init(&data->counter_mutex, NULL);
	data->request_counter = 0;
	data->running = false;
}
