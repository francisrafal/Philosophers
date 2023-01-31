/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frafal <frafal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 15:27:43 by frafal            #+#    #+#             */
/*   Updated: 2023/01/31 19:20:24 by frafal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	else
		data->eat_times = 0;
	if (invalid_nums(data))
		return (NULL);
	if (init_forks(data) == -1)
		return (NULL);
	if (pthread_mutex_init(&(data->alive_mutex), NULL))
		return (NULL);
	if (pthread_mutex_init(&(data->print_mutex), NULL))
		return (NULL);
	if (pthread_mutex_init(&(data->tv1_mutex), NULL))
		return (NULL);
	if (pthread_mutex_init(&(data->last_eaten_mutex), NULL))
		return (NULL);
	if (pthread_mutex_init(&(data->philos_mutex), NULL))
		return (NULL);
	if (init_last_eaten(data) == -1)
		return (NULL);
	return (data);
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
	{
		if (pthread_mutex_init(data->forks + i++, NULL))
			return (-1);
	}
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
