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

int	is_finished(t_table	*table)
{
	return (!table->ok || (table->input.n_to_eat != -1
			&& table->n_full == table->input.n_philos));
}

static void	kill_philo(t_table *table, int id)
{
	if (is_finished(table))
		return ;
	pthread_mutex_lock(&table->m_print);
	printf("%lu %d %s\n", get_time_ms() - table->t_start, id, MSG_DEATH);
	table->ok = 0;
}

static void	*watcher_routine(void *arg)
{
	t_philo	*philo;
	t_table	*table;
	int		id;

	philo = (t_philo *)arg;
	table = philo->table;
	id = philo->id;
	while (!is_finished(table))
	{
		pthread_mutex_lock(&philo->m_time);
		if ((int)(get_time_ms() - philo->t_last_meal) > table->input.t_to_die)
			break ;
		pthread_mutex_unlock(&philo->m_time);
		usleep(2000);
	}
	kill_philo(table, id);
	return (NULL);
}

static void	*philo_routine(void *arg)
{
	pthread_t	watcher;
	t_philo		*philo;
	int			meals_eaten;

	philo = (t_philo *)arg;
	meals_eaten = 0;
	pthread_mutex_lock(&philo->m_start);
	if (is_finished(philo->table))
		return (NULL);
	philo->t_last_meal = get_time_ms();
	if (pthread_create(&watcher, NULL, watcher_routine, philo))
		return (NULL);
	pthread_detach(watcher);
	philo->t_last_meal = get_time_ms();
	while (!is_finished(philo->table))
	{
		philo_eat(philo);
		++meals_eaten;
		check_fullness(philo->table, meals_eaten);
		philo_sleep(philo);
		philo_think(philo);
	}
	pthread_mutex_unlock(&philo->m_start);
	return (NULL);
}

int	create_threads(int n, t_philo *philos)
{
	int	i;

	i = -1;
	while (++i < n)
	{
		if (pthread_create(&philos[i].thread, NULL, philo_routine, philos + i))
		{
			print_error(ERRMSG_THREAD_CREATE, &philos[i].table->m_print);
			return (0);
		}
	}
	i = 0;
	while (i < n)
	{
		pthread_mutex_unlock(&philos[i].m_start);
		i += 2;
	}
	usleep(1000);
	i = 1;
	while (i < n)
	{
		pthread_mutex_unlock(&philos[i].m_start);
		i += 2;
	}
	return (1);
}
