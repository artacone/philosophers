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
#include <philo.h>

void	print_error(char *err_msg, pthread_mutex_t *lock)
{
	pthread_mutex_lock(lock);
	printf("%s", err_msg);
	pthread_mutex_unlock(lock);
}

void	print_msg(char *str, t_philo *philo)
{
	t_table	*table;

	table = philo->table;
	pthread_mutex_lock(&table->m_print);
	if (is_finished(table))
		return ;
	printf("%lu %d %s\n", get_time_ms() - table->t_start, philo->id, str);
	pthread_mutex_unlock(&table->m_print);
}
