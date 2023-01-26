/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frafal <frafal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 11:12:03 by frafal            #+#    #+#             */
/*   Updated: 2023/01/26 16:42:22 by frafal           ###   ########.fr       */
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
	data->last_eaten = malloc((data->num + 1)* sizeof (struct timeval));
	if (data->last_eaten == NULL)
	{
		printf("malloc fail\n");
		return (-1);
	}
	return (0);
}

int	init_philosophers(t_data *data)
{
	data->philos = malloc(data->num * sizeof (t_philo));
	if (data->philos == NULL)
	{
		printf("malloc fail\n");
		return (-1);
	}
	data->philosophers = malloc(data->num * sizeof (pthread_t));
	if (data->philosophers == NULL)
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
	if (init_philosophers(data) == -1)
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

int	max(int	a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

int	min(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}

void	*philosopher_thread(void *ptr)
{
	t_philo	*philo;
	t_data	*data;
	int		id;
	int		left;
	int		right;

	philo = (t_philo *)ptr;
	data = philo->data;
	id = philo->id;
	left = philo->left;
	right = philo->right;
	//printf("%d started\n", id);
	pthread_mutex_lock(&(data->alive_mutex));
	while (data->all_alive)
	{
		pthread_mutex_unlock(&(data->alive_mutex));
		pthread_mutex_lock(&(data->waiter));
		pthread_mutex_lock(&(data->alive_mutex));
		if (!data->all_alive)
		{
			pthread_mutex_unlock(&(data->waiter));
			break ;
		}
		pthread_mutex_unlock(&(data->alive_mutex));
		if (data->fork_availability[left] == FORK_FREE && data->fork_availability[right] == FORK_FREE)
		{
			data->fork_availability[left] = FORK_USED;
			data->fork_availability[right] = FORK_USED;
			pthread_mutex_lock(data->forks + min(left, right));
			printf("%ld %d has taken a fork\n", get_timestamp_in_ms(data), id);
			pthread_mutex_lock(data->forks + max(left, right));
			printf("%ld %d has taken a fork\n", get_timestamp_in_ms(data), id);
			printf("%ld %d is eating\n", get_timestamp_in_ms(data), id);
			pthread_mutex_lock(&(data->last_eaten_mutex));
			gettimeofday(data->last_eaten + id, NULL);
			pthread_mutex_unlock(&(data->last_eaten_mutex));
			usleep(data->tte * 1000);
			pthread_mutex_unlock(data->forks + max(left, right));
			pthread_mutex_unlock(data->forks + min(left, right));
			data->fork_availability[left] = FORK_FREE;
			data->fork_availability[right] = FORK_FREE;
			pthread_mutex_unlock(&(data->waiter));
			pthread_mutex_lock(&(data->alive_mutex));
			if (!data->all_alive)
				break ;
			pthread_mutex_unlock(&(data->alive_mutex));
			printf("%ld %d is sleeping\n", get_timestamp_in_ms(data), id);
			usleep(data->tts * 1000);
			pthread_mutex_lock(&(data->alive_mutex));
			if (!data->all_alive)
				break ;
			pthread_mutex_unlock(&(data->alive_mutex));
			printf("%ld %d is thinking\n", get_timestamp_in_ms(data), id);
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
	free_null(data->philosophers);
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

void	start_philosophers(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num)
	{
		data->philos[i].left = i;
		data->philos[i].right = (i + 1) % data->num;
		data->philos[i].id = i + 1;
		data->philos[i].data = data;
		pthread_create(&(data->philosophers[i]), NULL, philosopher_thread, data->philos + i);
		i++;
	}
}

void	*check_deaths(void *ptr)
{
	t_data	*data;
	int		id;

	data = (t_data *)ptr;
	id = 1;
	pthread_mutex_lock(&(data->alive_mutex));
	while (data->all_alive)
	{
		pthread_mutex_unlock(&(data->alive_mutex));
		while (id <= data->num)
		{
			// maybe mutex gettimeofday?
			gettimeofday(&(data->tv1), NULL);
			pthread_mutex_lock(&(data->last_eaten_mutex));
			if (time_diff_in_ms(data->tv1, data->last_eaten[id]) > data->ttd)
			{
				printf("time_diff_in_ms %ld, id %d \n", time_diff_in_ms(data->tv1, data->last_eaten[id]), id);
				printf("%ld %d died\n", get_timestamp_in_ms(data), id);
				pthread_mutex_lock(&(data->alive_mutex));
				data->all_alive = 0;
				pthread_mutex_unlock(&(data->alive_mutex));
				pthread_mutex_unlock(&(data->last_eaten_mutex));
				break ;
			}
			pthread_mutex_unlock(&(data->last_eaten_mutex));
			id++;
			usleep(1000);
		}
		pthread_mutex_lock(&(data->alive_mutex));
	}
	pthread_mutex_unlock(&(data->alive_mutex));
	return (NULL);
}

void	join_all_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num)
		pthread_join(data->philosophers[i++], NULL);
	pthread_join(data->death_thread, NULL);
}

void	set_last_eaten(t_data *data)
{
	int	id;

	id = 0;
	pthread_mutex_lock(&(data->last_eaten_mutex));
	while (id <= data->num)
		data->last_eaten[id++] = data->tv0;
	pthread_mutex_unlock(&(data->last_eaten_mutex));
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
	set_last_eaten(data);
	start_philosophers(data);
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
// Repair default repository for sync changes in gitlens
// Handle special cases like just one philosopher
// Update philosopher thread to use the correct forks
// Wrap alive checks with printf functions
// print mutex

// waiter should prioritize threads that haven't eaten in a long time (smalles last eaten time)