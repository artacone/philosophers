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
#include "../include/philo.h"

void	destroy_philos(int n, pid_t *philos)
{
	int	i;

	if (philos != NULL)
	{
		i = 0;
		while (i < n)
		{
			kill(philos[i], SIGKILL);
			++i;
		}
		free(philos);
	}
	sem_unlink(SEM_NAME_START);
	sem_unlink(SEM_NAME_END);
	sem_unlink(SEM_NAME_PRINT);
	sem_unlink(SEM_NAME_FORKS);
	sem_unlink(SEM_NAME_FULL);
	sem_unlink(SEM_NAME_TAKE);
}

int	end_simulation(t_table *table, pid_t *philos)
{
	int	n;

	sem_wait(table->sem_end);
	sem_wait(table->sem_end);
	n = table->input.n_philos;
	destroy_philos(n, philos);
	return (1);
}
