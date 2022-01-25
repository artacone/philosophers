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

static void	destroy_philos(int n, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&philos[i].m_start);
		pthread_mutex_destroy(&philos[i].m_time);
		pthread_detach(philos[i].thread);
		++i;
	}
}

static void	destroy_forks(int n, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(forks + i);
		++i;
	}
}

int	end_simulation(t_table *table, t_philo *philos)
{
	int	n;

	n = table->input.n_philos;
	destroy_philos(n, philos);
	destroy_forks(n, table->m_forks);
	pthread_mutex_destroy(&table->m_print);
	pthread_mutex_destroy(&table->m_fullness);
	return (1);
}
