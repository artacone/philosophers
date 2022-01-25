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

int	is_finished(t_philo *philo)
{
	return (*(philo->ok) || (philo->input->hungry == 0));
}

static void	take_fork(t_philo *philo, pthread_mutex_t *fork)
{
	if (!philo->is_alive)
		return ;
	pthread_mutex_lock(fork);
	print_msg(MSG_FORK, philo);
}

static void	eat(t_philo *philo)
{
	if (!philo->is_alive)
		return ;
	print_msg(MSG_EAT, philo);
	pthread_mutex_lock(philo->time);
	philo->t_last_meal = get_time_ms();
	pthread_mutex_unlock(philo->time);
	ms_sleep(philo->input->t_to_eat);
}

static void	eat_sleep_think(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		take_fork(philo, philo->first);
		take_fork(philo, philo->second);
	}
	else
	{
		take_fork(philo, philo->second);
		take_fork(philo, philo->first);
	}
	eat(philo);
	if (philo->meals_left > 0)
		philo->meals_left -= 1;
	print_msg(MSG_SLEEP, philo);
	pthread_mutex_unlock(philo->second);
	pthread_mutex_unlock(philo->first);
	ms_sleep(philo->input->t_to_sleep);
	if (!philo->is_alive)
		return ;
	print_msg(MSG_THINK, philo);
}

static void	kill_philo(t_philo *philo)
{
	if (is_finished(philo))
		return ;
	print_msg(MSG_DEATH, philo); // FIXME
	*(philo->ok) = 0;
}

static void	*watcher2(void *arg)
{
	t_philo	*philo;

	philo = arg;
	while (!is_finished(philo))
	{
		pthread_mutex_lock(philo->time);
		if ((int)(get_time_ms() - philo->t_last_meal) > philo->input->t_to_die)
			break ;
		pthread_mutex_unlock(philo->time);
		usleep(2000);
	}
	kill_philo(philo);
	return (NULL);
}

void	*philo_routine(void *arg)
{
	pthread_t	watcher;
	t_philo		*philo;

	philo = (t_philo *)arg;
	philo->t_last_meal = get_time_ms();
	if (pthread_create(&watcher, NULL, watcher2, philo) ||
			pthread_detach(watcher))
		return (NULL);
	philo->t_last_meal = get_time_ms();
	if (philo->id % 2 == 0)
		ms_sleep(1); // ???
	while (philo->is_alive)
	{
		eat_sleep_think(philo);
	}
	return (NULL);
}

int	has_died(t_philo *philo)
{
	pthread_mutex_lock(philo->time);
	if ((int)(get_time_ms() - philo->t_last_meal) > philo->input->t_to_die)
	{
		philo->is_alive = 0;
		pthread_mutex_unlock(philo->time);
		return (1);
	}
	pthread_mutex_unlock(philo->time);
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

int	check_philos(t_philo *philos, int n)
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
		if (philos[i].meals_left == 0)
		{
			philos[i].meals_left = -1;
			philos[i].input->hungry -= 1;
			if (philos[i].input->hungry == 0)
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
		if (!check_philos(philos, n))
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
		print_error(ERRMSG_THREAD_CREATE, &table->m_print);
		return (0);
	}
	i = 0;
	while (i < n)
	{
		if (pthread_create(table->threads + i, NULL, philo_routine, philos + i))
		{
			print_error(ERRMSG_THREAD_CREATE, &table->m_print);
			return (0);
		}
		if (pthread_detach(table->threads[i]))
		{
			print_error(ERRMSG_THREAD_DETACH, &table->m_print);
			return (0);
		}
		++i;
	}
	return (1);
}
