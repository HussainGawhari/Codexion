/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 16:52:01 by hgawhari          #+#    #+#             */
/*   Updated: 2026/08/20 12:47:22 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_event(t_data *data, unsigned int id, char *action)
{
	char	*color;

	color = RST;
	if (strcmp(action, STR_DONGLE) == 0)
		color = CYAN;
	else if (strcmp(action, STR_COMPILE) == 0)
		color = GREEN;
	else if (strcmp(action, STR_DEBUG) == 0)
		color = PURPLE;
	else if (strcmp(action, STR_REFACTOR) == 0)
		color = CYAN;
	else if (strcmp(action, STR_BURNOUT) == 0)
		color = RED;
	pthread_mutex_lock(&data->log_mutex);
	if (simulation_is_running(data))
		printf("%s%lu %u %s%s\n",
			color,
			get_time_ms() - data->start_time,
			id,
			action,
			RST);
	pthread_mutex_unlock(&data->log_mutex);
}
