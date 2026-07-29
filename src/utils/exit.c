/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 07:49:02 by hgawhari          #+#    #+#             */
/*   Updated: 2026/07/29 07:49:03 by hgawhari         ###   ########.fr       */
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

/* error_null:
 *	Frees any allocated memory,
		prints an error message and returns a NULL pointer.
 *	Used for error management during initialization.
 */
void	*error_null(char *str, char *details, void *table)
{
	(void)table;
	print_error(str, details);
	return (NULL);
}
