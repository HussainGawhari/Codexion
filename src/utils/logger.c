/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 16:52:01 by hgawhari          #+#    #+#             */
/*   Updated: 2026/08/09 16:10:57 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


void	log_action(t_data *data, unsigned int id, char *action)
{
	pthread_mutex_lock(&data->log_mutex);
	if (is_running(data))
		printf("%lu %u %s\n", get_time_ms() - data->start_time, id, action);
	pthread_mutex_unlock(&data->log_mutex);
}
