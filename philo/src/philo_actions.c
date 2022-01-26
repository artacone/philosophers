/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvertie <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 02:41:43 by rvertie           #+#    #+#             */
/*   Updated: 2022/01/26 02:41:45 by rvertie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	philo_take_fork(t_philo *philo, pthread_mutex_t *fork)
{
	if (is_finished(philo->table))
		return ;
	pthread_mutex_lock(fork);
	print_msg(MSG_FORK, philo);
}

void	check_fullness(t_table *table, int meals_eaten)
{
	if (!is_finished(table) && meals_eaten == table->input.n_to_eat)
	{
		pthread_mutex_lock(&table->m_fullness);
		++(table->n_full);
		pthread_mutex_unlock(&table->m_fullness);
	}
}

void	philo_eat(t_philo *philo)
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
	if (is_finished(philo->table))
		return ;
	pthread_mutex_lock(&philo->m_time);
	philo->t_last_meal = get_time_ms();
	pthread_mutex_unlock(&philo->m_time);
	print_msg(MSG_EAT, philo);
	ms_sleep(philo->table->input.t_to_eat);
}

void	philo_sleep(t_philo *philo)
{
	if (is_finished(philo->table))
		return ;
	print_msg(MSG_SLEEP, philo);
	pthread_mutex_unlock(philo->second);
	pthread_mutex_unlock(philo->first);
	ms_sleep(philo->table->input.t_to_sleep);
}

void	philo_think(t_philo *philo)
{
	if (is_finished(philo->table))
		return ;
	print_msg(MSG_THINK, philo);
	usleep(100);
}
