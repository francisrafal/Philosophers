/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frafal <frafal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 11:12:03 by frafal            #+#    #+#             */
/*   Updated: 2023/01/19 16:17:42 by frafal           ###   ########.fr       */
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

int	main(int argc, char **argv)
{
	t_data	*philo;

	if (!argc_correct(argc))
		return (1);
	philo = init_data(argc, argv);
	if (philo == NULL)
		return (1);
	return (0);
}
