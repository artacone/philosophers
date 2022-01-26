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

#include "../include/philo.h"
#include "../include/error.h"

int	main(int argc, char *argv[])
{
	t_table		table;
	pid_t		*pids;
	pthread_t	watcher;

	if (!init_table(argc, argv, &table, &pids))
	{
		return (ERROR_INPUT);
	}
	if (!create_threads(table.input.n_philos, philos))
	{
		return (ERROR_THREAD);
	}
	while (!is_finished(&table))
	{
		usleep(500);
	}
	if (!end_simulation(&table, philos))
	{
		return (ERROR_ENDSIM);
	}
	return (0);
}
