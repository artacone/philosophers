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
	t_philo	*philos;

	if (!init_table(argc, argv, &table, &philos))
	{
		return (ERROR_INPUT);
	}
	if (!create_threads(&table))
	{
		return (ERROR_THREAD);
	}
	if (!end_simulation(&table))
	{
		return (ERROR_ENDSIM);
	}
	return (0);
}
