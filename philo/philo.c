/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frafal <frafal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 11:12:03 by frafal            #+#    #+#             */
/*   Updated: 2023/01/31 19:26:23 by frafal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_data	*data;
	//t_philo	*philos;

	if (!argc_correct(argc))
		return (1);
	data = init_data(argc, argv);
	if (data == NULL)
		return (1);
	gettimeofday(&(data->tv0), NULL);
	set_last_eaten(data);
	data->philos = init_philosophers(data);
	if (data->philos == NULL)
	{
		free_data(data);
		return (1);
	}
	start_philosophers(data, data->philos);
	pthread_create(&(data->death_thread), NULL, check_deaths, data);
	join_all_threads(data);
	free_null(data->philos);
	if (free_data(data) == -1)
		return (1);
	return (0);
}

// Norminette