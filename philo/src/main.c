/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvertie <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/22 17:45:19 by rvertie           #+#    #+#             */
/*   Updated: 2022/01/22 17:45:22 by rvertie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <main.h>
#include <error.h>

int	main(int argc, char *argv[])
{
	t_table	table;

	// Initialize
	if (!init_table(argc, argv, &table))
	{
		return (ERROR_INPUT);
	}
	// Start threads
	if (!create_threads(&table))
	{
		return (2); // FIXME errcode
	}
	// Clear up (allocation -> free; destroy mutexes; ? join threads)

	return (0); // Consider returning status
}
