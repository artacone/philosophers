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
#include "../include/error.h"

static int	destroy_mutexes(int n, t_table *table)
{
	while (n > 0)
	{
		if (pthread_mutex_destroy(&table->m_forks[--n]) != 0)
		{
			return (0);
		}
	}
	if (pthread_mutex_destroy(&table->m_print) != 0)
	{
		return (0);
	}
	if (pthread_mutex_destroy(&table->m_fullness) != 0)
	{
		return (0);
	}
	return (1);
}
//
//static int
//	free_resources(t_philo *philos, pthread_t *ths, pthread_mutex_t *forks)
//{
//	free(philos);
//	free(ths);
//	free(forks);
//	return (1);
//}

int
	end_simulation(t_table *table)
{
	int	n;

	n = table->input.n_philos;

	if (!destroy_mutexes(n, table))
	{
		print_error(ERRMSG_MUTEX_DESTROY, &table->m_print);
		return (0);
	}
//	free_resources(table->philos, table->threads, table->m_forks);
	return (1);
}
