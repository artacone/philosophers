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
#include "../include/main.h"

void	print_error(char *err_msg)
{
	printf("%s", err_msg);
}

// Send only id and timestamp
void	print_msg(char *str, t_philo *philo, pthread_mutex_t *mutex_print)
{
	pthread_mutex_lock(mutex_print);
	printf("%lu %d %s\n", get_time_ms(), philo->id, str); // FIXME
	pthread_mutex_unlock(mutex_print);
}
