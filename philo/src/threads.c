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
#include "../include/error.h"

static void	eat_sleep_think(t_philo *philo)
{
	pthread_mutex_lock(philo->first);
	print_msg(MSG_FORK, philo); // FIXME
	pthread_mutex_lock(philo->second);
	print_msg(MSG_FORK, philo); // FIXME
	philo->last_meal_time = get_time_ms();
	print_msg(MSG_EAT, philo); // FIXME
	usleep(philo->input->time_to_eat * 1000);
	pthread_mutex_unlock(philo->second);
	pthread_mutex_unlock(philo->first);
	if (philo->meals_left > 0)
		philo->meals_left -= 1;
	print_msg(MSG_SLEEP, philo); // FIXME
	usleep(philo->input->time_to_sleep * 1000);
	print_msg(MSG_THINK, philo); // FIXME
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	philo->last_meal_time = get_time_ms();
	if (philo->id % 2 == 0)
	{
		usleep(philo->input->time_to_eat); // FIXME
	}
	while (philo->is_alive)
	{
		eat_sleep_think(philo);
	}
	return (NULL);
}

int	has_died(t_philo *philo)
{
	if ((int)(get_time_ms() - philo->last_meal_time) > philo->input->time_to_die)
	{
		philo->is_alive = 0;
		return (1);
	}
	return (0);
}

void	kill_all(t_philo *philos, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		philos[i++].is_alive = 0;
	}
}

int	check_philos(t_philo *philos, int n, int *hungry)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (has_died(philos + i))
		{
			print_msg(MSG_DEATH, philos + i);
			return (0);
		}
		if (philos->meals_left == 0)
		{
			philos->meals_left = -1;
			*hungry -= 1;
			if (*hungry == 0)
			{
				return (0);
			}
		}

		++i;
	}
	return (1);
}

void	*watcher_routine(void *arg)
{
	int		n;
	t_table	*table;
	t_philo	*philos;

	table = (t_table *)arg;
	n = table->input.n_philos;
	philos = table->philos;
	while (1)
	{
		if (!check_philos(philos, n, &table->hungry))
		{
			break ;
		}
	}
	kill_all(philos, n);
	return (NULL);
}

int	create_threads(t_table *table)
{
	int		i;
	int		n;
	t_philo	*philos;

	n = table->input.n_philos;
	philos = table->philos;
	if (pthread_create(&table->watcher, NULL, watcher_routine, table) != 0)
	{
		print_error(ERRMSG_THREAD_CREATE, &table->mutex_print);
		return (0);
	}
	i = 0;
	while (i < n)
	{
		if (pthread_create(table->threads + i, NULL, philo_routine, philos + i))
		{
			print_error(ERRMSG_THREAD_CREATE, &table->mutex_print);
			return (0);
		}
		++i;
	}
	return (1);
}
