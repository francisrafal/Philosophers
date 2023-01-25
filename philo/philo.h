/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frafal <frafal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 11:13:11 by frafal            #+#    #+#             */
/*   Updated: 2023/01/25 16:58:58 by frafal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/time.h>
# define EATING 1
# define SLEEPING 2
# define THINKING 3
# define DEAD 0
# define FORK_USED 0
# define FORK_FREE 1

typedef struct s_data
{
	int				num;
	int				ttd;
	int				tte;
	int				tts;
	int				eat_times;
	struct timeval	tv0;
	struct timeval	tv1;
	struct timeval	*last_eaten;
	pthread_t		tid1;
	pthread_t		tid2;
	pthread_t		death_thread;
	pthread_mutex_t	waiter;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	alive_mutex;
	pthread_mutex_t	last_eaten_mutex;
	int				*fork_availability;
	int				all_alive;
}	t_data;

typedef struct s_philo
{
	int				num;
	int				state;
	struct s_philo	*right;
}	t_philo;

typedef struct s_fork
{
	pthread_mutex_t	fork_mutex;
}	t_fork;

int	ft_atoi(const char *nptr);

#endif