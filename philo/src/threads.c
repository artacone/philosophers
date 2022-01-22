/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvertie <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/22 19:50:22 by rvertie           #+#    #+#             */
/*   Updated: 2022/01/22 19:50:23 by rvertie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/main.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		usleep(100);
		// take forks
		// eat
		// remember
	}
	return NULL;
}

void	*watcher_routine(void *arg)
{
	int		n;
	int		i;
	t_table	*table;

	table = (t_table *)arg;
	n = table->input.n_philos;
	while (1)
	{
		i = 0;
		while (i < n)
		{
			/*
			if (is_philo_dead())
			{
				// time
				// lock mutex
				// print
				return (0);
			}
			if (are_philos_full())
			{
				// End dinner
				return ((void *)1);
			}
			*/
			++i;
		}
	}
	return NULL;
}
#pragma clang diagnostic pop

int	create_threads(t_table *table)
{
	int	n;

	n = table->input.n_philos;

	if (pthread_create(&table->watcher, NULL, watcher_routine, table) != 0)
		return (0);
	while (n > 0)
	{
		pthread_create(&(table->philos[--n]), NULL, philo_routine, table);
	}
	return (1);
}
