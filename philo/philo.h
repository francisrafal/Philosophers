/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frafal <frafal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 11:13:11 by frafal            #+#    #+#             */
/*   Updated: 2023/01/30 17:26:41 by frafal           ###   ########.fr       */
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
	int				num_waiters;
	int				*fork_array;
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
int		philo_take_forks(t_data *data, t_philo *philo);
void	philo_put_forks(t_data *data, t_philo *philo);
void	philo_eat(t_data *data, t_philo *philo);
int		philo_sleep(t_data *data, t_philo *philo);
int		philo_think(t_data *data, t_philo *philo);
int		argc_correct(int argc);
int		still_alive(t_data *data);
void	gettimeofday_safe(t_data *data);
long	time_diff(struct timeval a, struct timeval b);
long	get_timestamp(t_data *data);
void	print_msg(int msg, t_data *data, int id);
int		init_forks(t_data *data);
int		init_waiters(t_data *data);
int		init_last_eaten(t_data *data);
t_philo	*init_philosophers(t_data *data);
t_data	*init_data(int argc, char **argv);
int		max(int	a, int b);
int		min(int a, int b);
int		my_turn(t_philo *philo);
void	*philosopher_thread(void *ptr);
void	free_null(void *ptr);
void	free_forks(t_data *data);
void	free_waiters(t_data *data);
void	free_data(t_data *data);
void	start_philosophers(t_data *data, t_philo *philos);
struct timeval	get_last_eaten(t_data *data, int id);
void	philo_dead(t_data *data, int id);
void	*check_deaths(void *ptr);
void	join_all_threads(t_data *data);
void	set_last_eaten(t_data *data);

#endif