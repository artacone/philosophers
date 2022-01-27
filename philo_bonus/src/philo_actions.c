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
#include "../include/philo.h"

void	philo_take_fork(t_philo *philo)
{
	sem_wait(philo->sem_forks);
	print_msg(MSG_FORK, philo);
}

void	check_fullness(t_table *table, int meals_eaten)
{
	if (meals_eaten == table->input.n_to_eat)
	{
		sem_post(table->sem_fullness);
	}
}

void	philo_eat(t_philo *philo)
{
	sem_wait(philo->sem_take);
	philo_take_fork(philo);
	philo_take_fork(philo);
	sem_post(philo->sem_take);
	sem_wait(philo->sem_time);
	philo->t_last_meal = get_time_ms();
	sem_post(philo->sem_time);
	print_msg(MSG_EAT, philo);
	ms_sleep(philo->table->input.t_to_eat);
}

void	philo_sleep(t_philo *philo)
{
	print_msg(MSG_SLEEP, philo);
	sem_post(philo->sem_forks);
	sem_post(philo->sem_forks);
	ms_sleep(philo->table->input.t_to_sleep);
}

void	philo_think(t_philo *philo)
{
	print_msg(MSG_THINK, philo);
	usleep(100);
}
