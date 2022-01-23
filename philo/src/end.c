/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvertie <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/23 17:46:28 by rvertie           #+#    #+#             */
/*   Updated: 2022/01/23 17:46:30 by rvertie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/main.h"

static int	join_threads(int n, pthread_t *philos, pthread_t *watcher)
{
	if (pthread_join(*watcher, NULL) != 0)
	{
		return (0);
	}
	while (n > 0)
	{
		if (pthread_join(philos[--n], NULL) != 0)
		{
			return (0);
		}
	}
	return (1);
}

static int	destroy_mutexes(int n, pthread_mutex_t *forks, pthread_mutex_t *print)
{
	while (n > 0)
	{
		if (pthread_mutex_destroy(&forks[--n]) != 0)
		{
			return (0);
		}
	}
	if (pthread_mutex_destroy(print) != 0)
	{
		return (0);
	}
	return (1);
}

static int	free_resources(t_philo *philos, pthread_t *ths, pthread_mutex_t *forks)
{
	free(philos);
	free(ths);
	free(forks);
	return (1);
}

int	end_simulation(t_table *table)
{
	int	n;

	n = table->input.n_philos;
	join_threads(n, table->threads, &table->watcher);
	destroy_mutexes(n, table->mutex_forks, &table->mutex_print);
	free_resources(table->philos, table->threads, table->mutex_forks);
	return (1);
}
