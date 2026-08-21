/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_dongles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 16:06:38 by hgawhari          #+#    #+#             */
/*   Updated: 2026/08/18 13:43:12 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
 * Function prepares all the shared resources so thread can use them later
 * This function initilize every dongle before simulation starts
 * since we have 1 dongle per coder so the number dongle == no of coders
 * cmp is pointer function that point to the function recevies two struct

	* every dongle has own mutex that protext dongles from simultaneous access.
 * coder is the thread and only one thread should dongle's protected states
 * pthread_cond_init allows coders to wait when dongles are not available
 * every dongle has it's own queue and cmp tells queue how to order requests.
 */
void	init_dongles(t_data *data)
{
	unsigned int	i;
	int				(*cmp)(t_task, t_task);

	if (data->scheduler == FIFO)
		cmp = cmp_fifo;
	else
		cmp = cmp_edf;
	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_init(&data->dongles[i].mutex, NULL);
		pthread_cond_init(&data->dongles[i].cond, NULL);
		data->dongles[i].available = true;
		data->dongles[i].last_release_ms = 0;
		queue_init(&data->dongles[i].wait_queue, cmp);
		i++;
	}
}
