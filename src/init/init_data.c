/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 16:06:33 by hgawhari          #+#    #+#             */
/*   Updated: 2026/07/24 11:50:05 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void init_data(t_data *data)
{
    init_dongles(data);
    init_coders(data);
    pthread_mutex_init(&data->log_mutex, NULL);
    pthread_mutex_init(&data->simulation_mutex, NULL);
    pthread_mutex_init(&data->counter_mutex, NULL);
    data->request_counter = 0;
    data->running = false;
}
