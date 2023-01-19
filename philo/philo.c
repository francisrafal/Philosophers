/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frafal <frafal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 11:12:03 by frafal            #+#    #+#             */
/*   Updated: 2023/01/19 16:51:08 by frafal           ###   ########.fr       */
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

t_data	*init_data(int argc, char **argv)
{
	t_data	*philo;

	philo = malloc(sizeof (t_data));
	if (philo == NULL)
	{
		printf("malloc fail\n");
		return (NULL);
	}
	philo->num = ft_atoi(argv[1]);
	philo->ttd = ft_atoi(argv[2]);
	philo->tte = ft_atoi(argv[3]);
	philo->tts = ft_atoi(argv[4]);
	if (argc == 6)
		philo->eat_times = ft_atoi(argv[5]);
	return (philo);
}

unsigned long	get_timestamp_in_ms(t_data *philo)
{
	unsigned long	sec_elapsed;
	unsigned long	usec_elapsed;
	unsigned long	msec_elapsed;

	gettimeofday(&(philo->tv1), NULL);
	sec_elapsed = philo->tv1.tv_sec - philo->tv0.tv_sec;
	usec_elapsed = philo->tv1.tv_usec - philo->tv0.tv_usec;
	msec_elapsed = sec_elapsed * 1000 + usec_elapsed / 1000;
	return (msec_elapsed);
}

int	main(int argc, char **argv)
{
	t_data	*philo;

	if (!argc_correct(argc))
		return (1);
	philo = init_data(argc, argv);
	gettimeofday(&(philo->tv0), NULL);
	if (philo == NULL)
		return (1);
	usleep(10000);
	printf("timestamp: %ld ms\n", get_timestamp_in_ms(philo));
	usleep(1000000);
	printf("timestamp: %ld ms\n", get_timestamp_in_ms(philo));
	return (0);
}
