/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 10:28:41 by hgawhari          #+#    #+#             */
/*   Updated: 2026/08/18 14:44:41 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
	* check every request dealine before, after, or same deadline
*/
int	cmp_edf(t_task a, t_task b)
{
	if (a.deadline < b.deadline)
		return (-1);
	if (a.deadline > b.deadline)
		return (1);
	if (a.coder_id > b.coder_id)
		return (-1);
	if (a.coder_id < b.coder_id)
		return (1);
	return (0);
}
