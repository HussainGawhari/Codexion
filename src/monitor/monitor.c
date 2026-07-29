/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 14:56:05 by hgawhari          #+#    #+#             */
/*   Updated: 2026/07/29 07:40:00 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	handle_burnout(t_data *data, unsigned int idx, unsigned long now)
{
	pthread_mutex_lock(&data->log_mutex);
	printf("%lu %u burn out \n", now - data->start_time, data->coders[idx].id);
	pthread_mutex_lock(&data->simulation_mutex);
	data->running = false;
	pthread_mutex_unlock(&data->simulation_mutex);
	pthread_mutex_unlock(&data->log_mutex);
	check_all(data);
}

static void	check_burnout(t_data *data)
{
	unsigned int	i;
	unsigned int	i;
	unsigned long	now;

	i = 0;
	while (i < data->number_of_coders)
	{
		i = 0;
		while (i < data->number_of_coders)
		{
			pthread_mutex_lock(&data->coders[i].mutex);
			if (data->coders[i].compiles_done >= data->number_of_compiles_required)
			{
				pthread_mutex_unlock(&data->coders[i++].mutex);
				continue ;
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
	t_data			*data;
	unsigned int	j;
	int				all_done;

	data = (t_data *)arg;
	while (is_running(data))
	{
		/* code */
		check_burnout(data);
		if (!is_running(data))
			break ;
		/* check if all coders finished their required compiles */
		{
			all_done = 1;
			j = 0;
			while (j < data->number_of_coders)
			{
				pthread_mutex_lock(&data->coders[j].mutex);
				if (data->coders[j].compiles_done < data->number_of_compiles_required)
					all_done = 0;
				pthread_mutex_unlock(&data->coders[j].mutex);
				if (!all_done)
					break ;
				j++;
			}
			if (all_done)
			{
				pthread_mutex_lock(&data->simulation_mutex);
				data->running = false;
				pthread_mutex_unlock(&data->simulation_mutex);
				check_all(data);
				break ;
			}
		}
		usleep(1000);
	}
	return (NULL);
}
