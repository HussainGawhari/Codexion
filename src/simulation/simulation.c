/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 19:47:05 by hgawhari          #+#    #+#             */
/*   Updated: 2026/08/17 19:57:26 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"

/*
    * Safely reads the shared running flag.
    * Check wether the simulation still running
    * Locks the mutex before reading data->running, because multiple threads can access it

*/

int simulation_is_running(t_data *data)
{
	int running;

	pthread_mutex_lock(&data->simulation_mutex);
	running = data->running;
	pthread_mutex_unlock(&data->simulation_mutex);
	return (running);
}

/*
    * Wakes up all the coders that might be currently waiting
    * Each coders has it's own conditional variables
    * pthread_cond_broadcast(&data->dongles[i].cond) wakes all threads waiting on that dongle's condition variable.
    * once all threads awaken so it checks [ pthread_con_wait() ]
    * 
*/
void wake_all_dongles(t_data *data)
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

/*
    * This function starts and manages simulations
    * Store the moment of that simulation starts
    * initialized coders time stamps
    * Creates a separate thread responsible for monitoring the simulation, especially burnout conditions
    * pthread_join() makes the main thread wait until each coder finishes
    * finally the main thread waits for the monitor thread to finish
    * coder_routine, &data->coders[i-1] this is argument that passed to codeer_routine.
*/

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
    while (i < data->number_of_coders)
    {
        pthread_create(
            &data->coders[i].thread,
            NULL,
            coder_routine,
            &data->coders[i]
        );
        i++;
    }
    wake_all_dongles(data);
    i = 0;
    while (i < data->number_of_coders)
    {
        pthread_join(data->coders[i].thread, NULL);
        i++;
    }
	 pthread_join(data->monitor_thread, NULL);

}
