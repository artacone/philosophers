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
	return (!table->ok || (table->input.n_to_eat != -1 &&
			table->n_full == table->input.n_philos));
}

static void	philo_take_fork(t_philo *philo, pthread_mutex_t *fork)
{
	if (is_finished(philo->table))
		return ;
	pthread_mutex_lock(fork);
	print_msg(MSG_FORK, philo);
}

static void	check_fullness(t_philo *philo, int meals_eaten)
{
	t_table	*table;

	table = philo->table;
	if (!is_finished(table) && philo->meals_eaten == table->input.n_to_eat)
	{
		pthread_mutex_lock(&table->m_fullness);
		++(table->n_full);
		pthread_mutex_unlock(&table->m_fullness);
	}
}

static void	philo_eat(t_philo *philo)
{
	if (is_finished(philo->table))
		return ;
	pthread_mutex_lock(&philo->m_time);
	philo->t_last_meal = get_time_ms();
	pthread_mutex_unlock(&philo->m_time);
	print_msg(MSG_EAT, philo);
	ms_sleep(philo->table->input.t_to_eat);
	++(philo->meals_eaten);
	check_fullness(philo, philo->meals_eaten);
}

static void	philo_sleep(t_philo *philo)
{
	if (is_finished(philo->table))
		return ;
	print_msg(MSG_SLEEP, philo);
	pthread_mutex_unlock(philo->second);
	pthread_mutex_unlock(philo->first);
	ms_sleep(philo->table->input.t_to_sleep);
}

static void	philo_think(t_philo *philo)
{
	if (is_finished(philo->table))
		return ;
	print_msg(MSG_THINK, philo);
}

static void	eat_sleep_think(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		philo_take_fork(philo, philo->first);
		philo_take_fork(philo, philo->second);
	}
	else
	{
		philo_take_fork(philo, philo->second);
		philo_take_fork(philo, philo->first);
	}
	philo_eat(philo);
	philo_sleep(philo);
	philo_think(philo);
}

static void	kill_philo(t_philo *philo)
{
	t_table	*table;

	table = philo->table;
	if (is_finished(table))
		return ;
	pthread_mutex_lock(&table->m_print); // FIXME
	printf("%lu %d %s\n",
		   get_time_ms() - table->t_start, philo->id, MSG_DEATH);
	table->ok = 0;
}

static void	*watcher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!is_finished(philo->table))
	{
		pthread_mutex_lock(&philo->m_time);
		if ((int)(get_time_ms() - philo->t_last_meal) > philo->table->input.t_to_die)
			break ;
		pthread_mutex_unlock(&philo->m_time);
		usleep(3000);
	}
	kill_philo(philo);
	return (NULL);
}

void	*philo_routine(void *arg)
{
	pthread_t	watcher;
	t_philo		*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->m_start);
	if (is_finished(philo->table))
		return (NULL);
	philo->t_last_meal = get_time_ms();
	if (pthread_create(&watcher, NULL, watcher_routine, philo))
		return (NULL);
	if (pthread_detach(watcher))
		return (NULL);
	philo->t_last_meal = get_time_ms();
	while (!is_finished(philo->table))
	{
		eat_sleep_think(philo);
	}
	return (NULL);
}

int	create_threads(int n, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (pthread_create(&philos[i].thread, NULL, philo_routine, philos + i))
		{
			print_error(ERRMSG_THREAD_CREATE, &philos[i].table->m_print);
			return (0);
		}
		if (pthread_detach(philos[i].thread))
		{
			print_error(ERRMSG_THREAD_DETACH, &philos[i].table->m_print);
			return (0);
		}
		++i;
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
