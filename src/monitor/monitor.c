/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 14:56:05 by hgawhari          #+#    #+#             */
/*   Updated: 2026/07/27 15:47:17 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void handle_burnout(t_data *data, unsigned int idx, unsigned long now)
{
	pthread_mutex_lock(&data->log_mutex);
	printf("%u %u burn out \n", now-data->start_time, data->coders[idx].id);
	pthread_mutex_lock(&data->simulation_mutex);
	data->running = false;
	pthread_mutex_unlock(&data->simulation_mutex);
	pthread_mutex_unlock(&data->log_mutex);
	check_all(data);
}

static void check_burnout(t_data *data)
{
	unsigned int i;

	i = 0;
	while (i < data->number_of_coders)
	{
		unsigned int i;
		unsigned long now;

		i = 0;
		while (i < data->number_of_coders)
		{
			pthread_mutex_lock(&data->coders[i].mutex);
			if (data->coders[i].compiles_done >= data->number_of_compiles_required)
			{
				pthread_mutex_unlock(&data->coders[i++].mutex);
				continue;
			}
			now = get_time_ms();
			if (now - data->coders[i].last_compile_ms >= data->time_to_burnout)
			{
				pthread_mutex_unlock(&data->coders[i].mutex);
				handle_burnout(data, i, now);
				return ;
			}
			pthread_mutex_unlock(&data->coders[i].mutex);
			i++;
		}


	}

}

void	*monitor_routine(void *arg)
{
	t_data *data;

	data = (t_data *)arg;
	while (is_running(data))
	{
		/* code */
		check_burout(data);
		if (!is_running(data))
			break;
		check_all_done(data);
		usleep(1000);
	}
	return (NULL);


}
