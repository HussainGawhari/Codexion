/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_coders.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 11:38:43 by hgawhari          #+#    #+#             */
/*   Updated: 2026/07/24 11:50:16 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_coders(t_data *data)
{
	 unsigned int i;

    i = 0;
    while (data->number_of_coders)
    {
        data->coders[i] = (t_coder){
            .id = i+1,
            .compiles_done = 0,
            .last_compile_ms = 0,
            .left = &data->dongles[i],
            .right = &data->dongles[i],
            .data = data,
        };
        pthread_mutex_init(&data->coders[i].mutex, NULL);
        i++;
    }
}
