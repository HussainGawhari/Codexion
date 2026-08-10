/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fifo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 10:26:47 by hgawhari          #+#    #+#             */
/*   Updated: 2026/08/10 19:23:40 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	cmp_fifo(t_task a, t_task b)
{
	if (a.arrival_order < b.arrival_order)
		return (-1);
	if (a.arrival_order > b.arrival_order)
		return (1);
	return (0);
}
