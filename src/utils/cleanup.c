/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 16:58:33 by hgawhari          #+#    #+#             */
/*   Updated: 2026/08/10 19:15:49 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void cleanup_mutexes(t_data *data)
{
	unsigned int i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_destroy(&data->coders[i].mutex);
		i++;
	}
	pthread_mutex_destroy(&data->log_mutex);
	pthread_mutex_destroy(&data->simulation_mutex);
	pthread_mutex_destroy(&data->counter_mutex);

}

static void cleanup_dongles(t_data *data)
{
	unsigned int i;

	if (!data->dongles)
		return ;
	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_destroy(&data->dongles[i].mutex);
		pthread_cond_destroy(&data->dongles[i].cond);
		queue_destroy(&data->dongles[i].wait_queue);
		i++;
	}

}

void 	cleanup(t_data *data)
{
	if (!data)
		return ;
	cleanup_dongles(data);
	cleanup_mutexes(data);
	if (data->coders)
	{
		free(data->coders);
		data->coders = NULL;
	}
	if (data->dongles)
	{
		free(data->dongles);
		data->dongles = NULL;
	}
}
