/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 14:56:05 by hgawhari          #+#    #+#             */
/*   Updated: 2026/08/18 07:52:09 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
	This function is called when check_burnout() detects that a coder has exceeded the burnout time.
*/

static void	handle_burnout(t_data *data, unsigned int idx, unsigned long now)
{
	pthread_mutex_lock(&data->log_mutex);
	printf("%lu %u burned out\n", now - data->start_time, data->coders[idx].id);
	pthread_mutex_lock(&data->simulation_mutex);
	data->running = false;
	pthread_mutex_unlock(&data->simulation_mutex);
	pthread_mutex_unlock(&data->log_mutex);
	wake_all_dongles(data);
}

/*
	This function checks every coder to see whether they have burned out.
	It goes through all coders one by one
*/

static void	check_burnout(t_data *data)
{
	unsigned int	i;
	unsigned long	now;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_lock(&data->coders[i].mutex);
		if (data->coders[i].compiles_done < data->number_of_compiles_required)
		{
			now = get_time_ms();
			if (now - data->coders[i].last_compile_ms >= data->time_to_burnout)
			{
				pthread_mutex_unlock(&data->coders[i].mutex);
				handle_burnout(data, i, now);
				return ;
			}
		}
		pthread_mutex_unlock(&data->coders[i].mutex);
		i++;
	}
}

/*
	This function checks whether all coders have completed their required compilations.
	Detect that the simulation has successfully finished and stop it.	
*/

static void	check_simulation_complete(t_data *data)
{
	unsigned int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_lock(&data->coders[i].mutex);
		if (data->coders[i].compiles_done < data->number_of_compiles_required)
		{
			pthread_mutex_unlock(&data->coders[i].mutex);
			return ;
		}
		pthread_mutex_unlock(&data->coders[i].mutex);
		i++;
	}
	pthread_mutex_lock(&data->simulation_mutex);
	data->running = false;
	pthread_mutex_unlock(&data->simulation_mutex);
	wake_all_dongles(data);
}

/*
	* This function is monitor thread and its job si to continuously watch the simulation.
	* while it is runing mainly for the burnout, and wake waiting coders when the simulation needs to finish.
	* It checks the coders' timing and determines whether a coder has exceeded
	* if someone burnout eventually it set to data->runign = false
*/

void	*monitor_simulation(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (simulation_is_running(data))
	{
		check_burnout(data);
		if (!simulation_is_running(data))
			break ;
		check_simulation_complete(data);
		usleep(1000);
		// Without this sleep, the monitor would continuously run check_burnout
	}
	return (NULL);
}
