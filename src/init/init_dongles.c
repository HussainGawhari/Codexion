/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_dongles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 16:06:38 by hgawhari          #+#    #+#             */
/*   Updated: 2026/08/10 19:23:40 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void init_dongles(t_data *data)
{
    unsigned int i;
    int (*cmp)(t_task, t_task);
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
