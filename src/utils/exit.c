/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 07:49:02 by hgawhari          #+#    #+#             */
/*   Updated: 2026/08/18 10:11:04 by hgawhari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	print_error(const char *msg, char *details)
{
	if (details && *details)
		printf(msg, details);
	else
		printf("%s\n", msg);
	return (0);
}
