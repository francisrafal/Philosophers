/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frafal <frafal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 15:37:22 by frafal            #+#    #+#             */
/*   Updated: 2023/01/31 19:10:08 by frafal           ###   ########.fr       */
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

int	invalid_nums(t_data *data)
{
	int	invalid;

	invalid = 0;
	if (data->num < 1)
		invalid++;
	if (data->ttd < 0)
		invalid++;
	if (data->tte < 0)
		invalid++;
	if (data->tts < 0)
		invalid++;
	if (data->eat_times < 0)
		invalid++;
	if (invalid)
		free(data);
	return (invalid);
}
