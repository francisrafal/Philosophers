/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frafal <frafal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 11:13:11 by frafal            #+#    #+#             */
/*   Updated: 2023/01/30 13:36:15 by frafal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/time.h>
# define MSG_TAKE_FORK 0
# define MSG_EATING 1
# define MSG_SLEEPING 2
# define MSG_THINKING 3
# define MSG_DIED 4

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
	pthread_t		*philo_threads;
	pthread_t		death_thread;
	pthread_mutex_t	*waiters;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	tv1_mutex;
	pthread_mutex_t	alive_mutex;
	pthread_mutex_t	queue_mutex;
	pthread_mutex_t	last_eaten_mutex;
	int				all_alive;
	int				*queue;
}	t_data;

typedef struct s_philo
{
	int				id;
	int				left;
	int				right;
	int				waiter_id;
	struct s_data	*data;
}	t_philo;

typedef struct s_fork
{
	pthread_mutex_t	fork_mutex;
}	t_fork;

int		ft_atoi(const char *nptr);
int		*init_queue(t_data *data);
void	print_queue(t_data *data);

#endif