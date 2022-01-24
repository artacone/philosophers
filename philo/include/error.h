/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvertie <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/22 17:45:03 by rvertie           #+#    #+#             */
/*   Updated: 2022/01/22 17:45:27 by rvertie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# define ERROR_INPUT	1
# define ERROR_THREAD	2
# define ERROR_ENDSIM	3

# define ERRMSG_THREAD_CREATE	"Error: thread creation failed.\n"
# define ERRMSG_THREAD_JOIN		"Error: thread join failed.\n"
# define ERRMSG_THREAD_DETACH	"Error: thread detach failed.\n"
# define ERRMSG_MUTEX_DESTROY	"Error: mutex destroy failed.\n"

#endif
