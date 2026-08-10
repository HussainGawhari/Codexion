/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 19:47:05 by hgawhari          #+#    #+#             */
/*   Updated: 2026/08/10 19:52:12 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"

int simulation_is_running(t_data *data)
{
	int running;

	pthread_mutex_lock(&data->simulation_mutex);
	running = data->running;
	pthread_mutex_unlock(&data->simulation_mutex);
	return (running);
}

void check_simulation_end(t_data *data)
{
    unsigned int i;

    i = 0;
    while (i < data->number_of_coders)
    {
        /* code */
        pthread_mutex_lock(&data->dongles[i].mutex);
        pthread_cond_broadcast(&data->dongles[i].cond);
        pthread_mutex_unlock(&data->dongles[i].mutex);
        i++;
    }

}

void start_simulation(t_data *data)
{
    unsigned int i;

    data->start_time = get_time_ms();
    i = 0;
    while (i < data->number_of_coders)
    {
        data->coders[i].last_compile_ms = data->start_time;
        i++;
    }
    data->running = true;
    pthread_create(&data->monitor_thread, NULL, monitor_simulation, data);
    i = 0;
    while ( i++ < data->number_of_coders)
    {
        pthread_create(&data->coders[i-1].thread, NULL, coder_routine, &data->coders[i-1]);
    }
    check_simulation_end(data);
    i = 0;
    while (i < data->number_of_coders)
    {
        pthread_join(data->coders[i].thread, NULL);
        i++;
    }
	 pthread_join(data->monitor_thread, NULL);

}
