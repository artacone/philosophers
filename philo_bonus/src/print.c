/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvertie <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/23 18:21:36 by rvertie           #+#    #+#             */
/*   Updated: 2022/01/23 18:21:38 by rvertie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/philo.h"

void	print_error(char *err_msg, pthread_mutex_t *lock)
{
	pthread_mutex_lock(lock);
	printf("%s", err_msg);
	pthread_mutex_unlock(lock);
}

void	print_msg(char *str, t_philo *philo)
{
	size_t	timestamp;

	timestamp = get_time_ms() - philo->table->t_start;
	sem_wait(philo->sem_print);
	printf("%lu %d %s\n", timestamp, philo->id, str);
	sem_post(philo->sem_print);
}
