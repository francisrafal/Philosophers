/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frafal <frafal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 11:12:03 by frafal            #+#    #+#             */
/*   Updated: 2023/01/25 17:21:03 by frafal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	argc_correct(int argc)
{
	if (argc != 5 && argc != 6)
	{
		printf("usage: ./philo number_of_philosophers \
time_to_die \
time_to_eat \
time_to_sleep \
[number_of_times_each_philosopher_must_eat]\n");
		return (0);
	}
	return (1);
}

int	init_fork_mutex(t_data *data)
{
	int	i;

	data->forks = malloc(data->num * sizeof (pthread_mutex_t));
	if (data->forks == NULL)
	{
		printf("malloc fail\n");
		return (-1);
	}
	i = 0;
	while (i < data->num)
		pthread_mutex_init(data->forks + i++, NULL);
	return (0);
}

int	init_fork_availability(t_data *data)
{
	int	i;

	data->fork_availability = malloc(data->num * sizeof (int));
	if (data->fork_availability == NULL)
	{
		printf("malloc fail\n");
		return (-1);
	}
	i = 0;
	while (i < data->num)
		data->fork_availability[i++] = FORK_FREE;
	return (0);
}

int	init_last_eaten(t_data *data)
{
	data->last_eaten = malloc(data->num * sizeof (struct timeval));
	if (data->last_eaten == NULL)
	{
		printf("malloc fail\n");
		return (-1);
	}
	return (0);
}

t_data	*init_data(int argc, char **argv)
{
	t_data	*data;

	data = malloc(sizeof (t_data));
	if (data == NULL)
	{
		printf("malloc fail\n");
		return (NULL);
	}
	data->num = ft_atoi(argv[1]);
	data->ttd = ft_atoi(argv[2]);
	data->tte = ft_atoi(argv[3]);
	data->tts = ft_atoi(argv[4]);
	data->all_alive = 1;
	if (argc == 6)
		data->eat_times = ft_atoi(argv[5]);
	if (init_fork_mutex(data) == -1)
		return (NULL);
	if (init_fork_availability(data) == -1)
		return (NULL);
	pthread_mutex_init(&(data->waiter), NULL);
	pthread_mutex_init(&(data->alive_mutex), NULL);
	pthread_mutex_init(&(data->print_mutex), NULL);
	pthread_mutex_init(&(data->last_eaten_mutex), NULL);
	if (init_last_eaten(data) == -1)
		return (NULL);
	return (data);
}

long	time_diff_in_ms(struct timeval a, struct timeval b)
{
	return ((a.tv_sec - b.tv_sec) * 1000 + (a.tv_usec - b.tv_usec) / 1000);
}

long	get_timestamp_in_ms(t_data *data)
{
	gettimeofday(&(data->tv1), NULL);
	return (time_diff_in_ms(data->tv1, data->tv0));
}

void	*philosopher_thread(void *ptr)
{
	t_data	*data;

	data = (t_data *)ptr;
	pthread_mutex_lock(&(data->last_eaten_mutex));
	data->last_eaten[0] = data->tv0;
	pthread_mutex_unlock(&(data->last_eaten_mutex));
	pthread_mutex_lock(&(data->alive_mutex));
	while (data->all_alive)
	{
		pthread_mutex_unlock(&(data->alive_mutex));
		pthread_mutex_lock(&(data->waiter));
		if (data->fork_availability[0] == FORK_FREE && data->fork_availability[1] == FORK_FREE)
		{
			data->fork_availability[0] = FORK_USED;
			data->fork_availability[1] = FORK_USED;
			pthread_mutex_lock(data->forks + 0);
			printf("%ld 0 has taken a fork\n", get_timestamp_in_ms(data));
			pthread_mutex_lock(data->forks + 1);
			printf("%ld 0 has taken a fork\n", get_timestamp_in_ms(data));
			printf("%ld 0 is eating\n", get_timestamp_in_ms(data));
			usleep(data->tte * 1000);
			pthread_mutex_unlock(data->forks + 0);
			pthread_mutex_unlock(data->forks + 1);
			data->fork_availability[0] = FORK_FREE;
			data->fork_availability[1] = FORK_FREE;
			pthread_mutex_unlock(&(data->waiter));
			pthread_mutex_lock(&(data->last_eaten_mutex));
			gettimeofday(data->last_eaten + 0, NULL);
			pthread_mutex_unlock(&(data->last_eaten_mutex));
			printf("%ld 0 is sleeping\n", get_timestamp_in_ms(data));
			usleep(data->tts * 1000);
			printf("%ld 0 is thinking\n", get_timestamp_in_ms(data));
		}
		pthread_mutex_lock(&(data->alive_mutex));
	}
	pthread_mutex_unlock(&(data->alive_mutex));
	return (NULL);
}

void	free_null(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
}

void	free_fork_mutex(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num)
		pthread_mutex_destroy(data->forks + i++);
	free_null(data->forks);
}

void	free_data(t_data *data)
{
	free_null(data->fork_availability);
	free_fork_mutex(data);
	pthread_mutex_destroy(&(data->waiter));
	pthread_mutex_destroy(&(data->print_mutex));
	pthread_mutex_destroy(&(data->alive_mutex));
	pthread_mutex_destroy(&(data->last_eaten_mutex));
	free_null(data->last_eaten);
	free_null(data);
}

/* void	print_error_exit(char *errmsg, t_data *data)
{
	ft_putstr_fd(errmsg, 2);
	free_data(data);
	exit(EXIT_FAILURE);
} */

void	init_philosophers(t_data *data)
{
	pthread_create(&(data->tid1), NULL, philosopher_thread, data);
}

void	*check_deaths(void *ptr)
{
	t_data	*data;

	data = (t_data *)ptr;
	while (1)
	{
		gettimeofday(&(data->tv1), NULL);
		pthread_mutex_lock(&(data->last_eaten_mutex));
		if (time_diff_in_ms(data->tv1, data->last_eaten[0]) > data->ttd)
		{
			printf("%ld 0 died\n", get_timestamp_in_ms(data));
			pthread_mutex_lock(&(data->alive_mutex));
			data->all_alive = 0;
			pthread_mutex_unlock(&(data->alive_mutex));
			break ;
		}
		pthread_mutex_unlock(&(data->last_eaten_mutex));
		usleep(5000);
	}
	return (NULL);
}

void	join_all_threads(t_data *data)
{
	pthread_join(data->tid1, NULL);
	pthread_join(data->death_thread, NULL);
}

int	main(int argc, char **argv)
{
	t_data	*data;

	if (!argc_correct(argc))
		return (1);
	data = init_data(argc, argv);
	if (data == NULL)
		return (1);
	gettimeofday(&(data->tv0), NULL);
	init_philosophers(data);
	pthread_create(&(data->death_thread), NULL, check_deaths, data);
	join_all_threads(data);
	free_data(data);
	return (0);
}

// Protect all system calls and perror exit
// Protect all mallocs
// Free Data at the end of main
// Check memory Leaks
// Check Data Races
// Check if arguments are positive
// Norminette
// print mutex
// alive mutex
// Repair destroy locked mutex
// Repair default repository for sync changes in gitlens