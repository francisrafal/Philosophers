/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frafal <frafal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 11:12:03 by frafal            #+#    #+#             */
/*   Updated: 2023/01/30 15:41:03 by frafal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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