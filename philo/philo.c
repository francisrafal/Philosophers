/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frafal <frafal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 11:12:03 by frafal            #+#    #+#             */
/*   Updated: 2023/01/25 11:16:25 by frafal           ###   ########.fr       */
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

int	init_forks(t_data *data)
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
		pthread_mutex_init(data->fork + i++, NULL);
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
	if (argc == 6)
		data->eat_times = ft_atoi(argv[5]);
	if (init_forks(data) == -1)
		return (NULL);
	return (data);
}

unsigned long	get_timestamp_in_ms(t_data *data)
{
	unsigned long	sec_elapsed;
	unsigned long	usec_elapsed;
	unsigned long	msec_elapsed;

	gettimeofday(&(data->tv1), NULL);
	sec_elapsed = data->tv1.tv_sec - data->tv0.tv_sec;
	usec_elapsed = data->tv1.tv_usec - data->tv0.tv_usec;
	msec_elapsed = sec_elapsed * 1000 + usec_elapsed / 1000;
	return (msec_elapsed);
}

void	*thread_routine(void *ptr)
{
	pthread_t	tid;

	(void)ptr;
	tid = pthread_self();
	printf("Thread: %ld\n", tid);
	return (NULL);
}

void	perror_exit(char *err)
{
	perror(err);
	exit(EXIT_FAILURE);
}

void	free_str_arr(char **str_arr)
{
	int	i;

	i = 0;
	while (str_arr[i] != NULL)
	{
		free_null(str_arr[i]);
		i++;
	}
	free_null(str_arr);
}

void	free_null(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
}

void	free_forks(t_data *data)
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
	free_forks(data);
	free_null(data);
}

void	print_error_exit(char *errmsg, t_data *data)
{
	ft_putstr_fd(errmsg, 2);
	free_data(data);
	exit(EXIT_FAILURE);
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
	pthread_create(&(data->tid1), NULL, thread_routine, NULL);
	printf("Main: Created first thread %ld\n", data->tid1);
	pthread_create(&(data->tid2), NULL, thread_routine, NULL);
	printf("Main: Created second thread %ld\n", data->tid2);
	pthread_join(data->tid1, NULL);
	printf("Main: Joining first thread %ld\n", data->tid1);
	pthread_join(data->tid2, NULL);
	printf("Main: Joining second thread %ld\n", data->tid2);
	free_data(data);
	return (0);
}

// Protect all system calls and perror exit
// Protect all mallocs
// Free Data at the end of main