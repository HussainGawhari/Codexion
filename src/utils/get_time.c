/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_time.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 14:49:54 by hgawhari          #+#    #+#             */
/*   Updated: 2026/07/24 14:54:20 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

unsigned long get_time_ms(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000UL) + (tv.tv_usec / 1000UL));
}

void	ft_usleep(unsigned long ms, t_data *data)
{
	unsigned long start_time;

	start_time = get_time_ms();
	while (get_time_ms() - start_time < ms)
	{
		/* code */
		if (!is_running(data))
			break;
		usleep(500);
	}

}
