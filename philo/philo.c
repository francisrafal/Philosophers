/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frafal <frafal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 11:12:03 by frafal            #+#    #+#             */
/*   Updated: 2023/01/30 14:08:52 by frafal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	argc_correct(int argc)
{
	if (argc != 5 && argc != 6)
	{
		printf(
			"usage: ./philo number_of_philosophers " \
			"time_to_die " \
			"time_to_eat " \
			"time_to_sleep " \
			"[number_of_times_each_philosopher_must_eat]\n");
		return (0);
	}
	return (1);
}

int	still_alive(t_data *data)
{
	pthread_mutex_lock(&(data->alive_mutex));
	if (!data->all_alive)
	{
		pthread_mutex_unlock(&(data->alive_mutex));
		return (0);
	}
	pthread_mutex_unlock(&(data->alive_mutex));
	return (1);
}

void	gettimeofday_safe(t_data *data)
{
	pthread_mutex_lock(&(data->tv1_mutex));
	gettimeofday(&(data->tv1), NULL);
	pthread_mutex_unlock(&(data->tv1_mutex));
}

long	time_diff(struct timeval a, struct timeval b)
{
	return ((a.tv_sec - b.tv_sec) * 1000 + (a.tv_usec - b.tv_usec) / 1000);
}

long	get_timestamp(t_data *data)
{
	gettimeofday_safe(data);
	return (time_diff(data->tv1, data->tv0));
}

void	print_msg(int msg, t_data *data, int id)
{
	long	time;

	time = get_timestamp(data);
	pthread_mutex_lock(&(data->print_mutex));
	if (msg == MSG_TAKE_FORK)
		printf("%ld %d has taken a fork\n", time, id);
	else if (msg == MSG_EATING)
		printf("%ld %d is eating\n", time, id);
	else if (msg == MSG_SLEEPING)
		printf("%ld %d is sleeping\n", time, id);
	else if (msg == MSG_THINKING)
		printf("%ld %d is thinking\n", time, id);
	else if (msg == MSG_DIED)
		printf("%ld %d died\n", time, id);
	pthread_mutex_unlock(&(data->print_mutex));
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
		pthread_mutex_init(data->forks + i++, NULL);
	return (0);
}

int	init_waiters(t_data *data)
{
	int	i;

	if (data->num == 1)
		data->num_waiters = 1;
	else
		data->num_waiters = data->num / 2;
	data->waiters = malloc(data->num_waiters * sizeof (pthread_mutex_t));
	if (data->waiters == NULL)
	{
		printf("malloc fail\n");
		return (-1);
	}
	i = 0;
	while (i < data->num_waiters)
		pthread_mutex_init(data->waiters + i++, NULL);
	return (0);
}

int	init_last_eaten(t_data *data)
{
	data->last_eaten = malloc((data->num + 1) * sizeof (struct timeval));
	if (data->last_eaten == NULL)
	{
		printf("malloc fail\n");
		return (-1);
	}
	return (0);
}

t_philo	*init_philosophers(t_data *data)
{
	t_philo	*philos;

	philos = malloc(data->num * sizeof (t_philo));
	if (philos == NULL)
	{
		printf("malloc fail\n");
		return (NULL);
	}
	data->philo_threads = malloc(data->num * sizeof (pthread_t));
	if (data->philo_threads == NULL)
	{
		printf("malloc fail\n");
		return (NULL);
	}
	return (philos);
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
	if (init_forks(data) == -1)
		return (NULL);
	if (init_waiters(data) == -1)
		return (NULL);
	pthread_mutex_init(&(data->alive_mutex), NULL);
	pthread_mutex_init(&(data->print_mutex), NULL);
	pthread_mutex_init(&(data->tv1_mutex), NULL);
	pthread_mutex_init(&(data->last_eaten_mutex), NULL);
	pthread_mutex_init(&(data->queue_mutex), NULL);
	if (init_last_eaten(data) == -1)
		return (NULL);
	return (data);
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

int	my_turn(t_philo *philo)
{
	int	i;

	if (!still_alive(philo->data))
		return (1);
	pthread_mutex_lock(&(philo->data->queue_mutex));
	// DOES THIS MAKE SENSE?
	i = philo->waiter_id;
	if (philo->id == philo->data->queue[i])
	{
		// DOES THIS MAKE SENSE?
		philo->waiter_id = i;
		if (philo->data->queue[i] == philo->data->num)
			philo->data->queue[i] = 1;
		else
			philo->data->queue[i] = philo->data->queue[i] + 1;
		pthread_mutex_unlock(&(philo->data->queue_mutex));
		return (1);
	}
	pthread_mutex_unlock(&(philo->data->queue_mutex));
	return (0);
}

void	*philosopher_thread(void *ptr)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)ptr;
	data = philo->data;
	while (still_alive(data))
	{
		while (!my_turn(philo))
			;
		pthread_mutex_lock(&(data->waiters[philo->waiter_id]));
		if (philo_take_forks(data, philo) == -1)
		{
			pthread_mutex_unlock(&(data->waiters[philo->waiter_id]));
			break ;
		}
		philo_eat(data, philo);
		philo_put_forks(data, philo);
		pthread_mutex_unlock(&(data->waiters[philo->waiter_id]));
		if (philo_sleep(data, philo) == -1)
			break ;
		if (philo_think(data, philo) == -1)
			break ;
	}
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

void	free_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num)
		pthread_mutex_destroy(data->forks + i++);
	free_null(data->forks);
}

void	free_waiters(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_waiters)
		pthread_mutex_destroy(data->waiters + i++);
	free_null(data->waiters);
}

void	free_data(t_data *data)
{
	free_null(data->philo_threads);
	free_forks(data);
	free_waiters(data);
	pthread_mutex_destroy(&(data->print_mutex));
	pthread_mutex_destroy(&(data->tv1_mutex));
	pthread_mutex_destroy(&(data->alive_mutex));
	pthread_mutex_destroy(&(data->last_eaten_mutex));
	pthread_mutex_destroy(&(data->queue_mutex));
	free_null(data->queue);
	free_null(data->last_eaten);
	free_null(data);
}

void	start_philosophers(t_data *data, t_philo *philos)
{
	int	i;
	int	j;

	j = 0;
	while (j < 2)
	{
		i = j;
		while (i < data->num)
		{
			philos[i].left = i;
			philos[i].right = (i + 1) % data->num;
			philos[i].id = i + 1;
			philos[i].data = data;
			philos[i].waiter_id = i / 2;
			pthread_create(&(data->philo_threads[i]), NULL, philosopher_thread, philos + i);
			i = i + 2;
		}
		j++;
	}
}

struct timeval	get_last_eaten(t_data *data, int id)
{
	struct timeval	last_eaten;

	pthread_mutex_lock(&(data->last_eaten_mutex));
	last_eaten = data->last_eaten[id];
	pthread_mutex_unlock(&(data->last_eaten_mutex));
	return (last_eaten);
}

void	philo_dead(t_data *data, int id)
{
	print_msg(MSG_DIED, data, id);
	pthread_mutex_lock(&(data->alive_mutex));
	data->all_alive = 0;
	pthread_mutex_unlock(&(data->alive_mutex));
}

void	*check_deaths(void *ptr)
{
	t_data	*data;
	int		id;

	data = (t_data *)ptr;
	while (still_alive(data))
	{
		id = 1;
		while (id <= data->num)
		{
			gettimeofday_safe(data);
			if (time_diff(data->tv1, get_last_eaten(data, id)) > data->ttd)
			{
				philo_dead(data, id);
				break ;
			}
			id++;
			usleep(2000);
		}
	}
	return (NULL);
}

void	join_all_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num)
		pthread_join(data->philo_threads[i++], NULL);
	pthread_join(data->death_thread, NULL);
}

void	set_last_eaten(t_data *data)
{
	int	id;

	id = 1;
	pthread_mutex_lock(&(data->last_eaten_mutex));
	while (id <= data->num)
		data->last_eaten[id++] = data->tv0;
	pthread_mutex_unlock(&(data->last_eaten_mutex));
}

int	main(int argc, char **argv)
{
	t_data	*data;
	t_philo	*philos;

	if (!argc_correct(argc))
		return (1);
	data = init_data(argc, argv);
	if (data == NULL)
		return (1);
	gettimeofday(&(data->tv0), NULL);
	set_last_eaten(data);
	if (init_queue(data) == NULL)
	{
		free_data(data);
		return (1);
	}
	// print_queue(data);
	philos = init_philosophers(data);
	if (philos == NULL)
	{
		free_data(data);
		return (1);
	}
	start_philosophers(data, philos);
	pthread_create(&(data->death_thread), NULL, check_deaths, data);
	join_all_threads(data);
	free_data(data);
	free_null(philos);
	return (0);
}

// Protect all system calls and perror exit
// Protect all mallocs
// Free Data at the end of main
// Check memory Leaks
// Check Data Races
// Check if arguments are positive
// Check if arguments are valid numbers and not exceeding max ms?
// Norminette
// test with --tool=helgrind
// test with --tool=drd
// remove fsanitize=thread

// Handle special cases like just one philosopher

// Solve parallelism
// Maybe two or multiple waiters to increase parallelism?
// 3 Philo: 1 Waiter
// 4 Philo: 2 Waiters
// 5 Philo: 2 Waiters
// 6 Philo: 3 Waiters
// 7 Philo: 3 Waiters
// 8 Philo: 4 Waiter
// 1, 2, 3, 4, 5
// 3, 4, 5, 1, 2

// 1, 2, 3, 4
// 3, 4, 1, 2

// 1, 2, 3, 4, 5, 6
// 3, 4, 5, 6, 1, 2
// 5, 6, 1, 2, 3, 4

// 1, 2, 3, 4, 5, 6, 7
// 3, 4, 5, 6, 7, 1, 2
// 5, 6, 7, 1, 2, 3, 4


// Handle number_of_times_each_philosopher_must_eat
// Problems with 5, 6 and 7 waiters
// Should it really depend on how I start the threads?
// Remove philo executable in remote repository